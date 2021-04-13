/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cassert>
#include "../include/tetris.hpp"
#include "../include/Jeu.hpp"
#include "../include/Piece.hpp"
#define SIZE_BLOC 35
#define BLOCSX 10
#define BLOCSY 20

#define FREE_SURFACE(surface_t) { SDL_FreeSurface(surface_t); surface_t = nullptr;}
#define FREE_TEXTURE(texture_t) { SDL_DestroyTexture(texture_t); texture_t = nullptr;}
#define FREE_RENDERER_AND_WINDOW(renderer_t, window_t) { \
SDL_DestroyRenderer(renderer_t); SDL_DestroyWindow(window_t); \
renderer_t = nullptr; window_t = nullptr;}

void Jeu::startTetris(int h,int w, SDL_Rect sizeTetris, bool multiplayer){

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) // Initialisation de la SDL
	{
		printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else{
		//on optimise l'espace en trouvant la dimension qui fait défaut
		//le tetris a un rapport de BLOCSX par BLOCSY.
		int minFactor = dm.w / (3*BLOCSX) < dm.h / BLOCSY ? dm.w / (3*BLOCSX) : dm.h / BLOCSY;
		w = 3*BLOCSX * minFactor;
		h = BLOCSY * minFactor;

		//on prend seulement une partie de lécran
		w = (int)((float)(w)*(4./5.));
		h = (int)((float)(h)*(4./5.));

		//on passe les paramètres à sizeTetris
		sizeTetris.w = w/3 ;
		sizeTetris.w = (sizeTetris.w/10) *10; //sizeTetris.w doit être un multple de BLOCSX = 10
		sizeTetris.h = sizeTetris.w * 2;

		w = sizeTetris.w * 3;
		h = sizeTetris.h;

		sizeTetris.x = sizeTetris.w;
		Piece::initStaticMembers(sizeTetris);

	}

	if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	 }
	//Mix_VolumeMusic(MIX_MAX_VOLUME / 2); //Mettre le volume à la moitié
	this->music = Mix_LoadMUS("sfx/tetris.mp3");

	if (music == nullptr)
	{
	    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
	    Mix_CloseAudio();
		Mix_Quit();
	    SDL_Quit();
	    return;
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME/2);

	if(Mix_PlayMusic(music, -1)==-1) {
    	printf("Mix_PlayMusic: %s\n", Mix_GetError());
		// well, there's no music, but most games don't break without music...
	}

	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	bool quitgame = false;
	while(!quitgame)
		quitgame = Jeu::MenuLancement(h,w,music, sizeTetris);
//	Mix_FreeMusic(music); //Libération de la musique
	Mix_FreeMusic(this->music);
   	Mix_CloseAudio(); //Fermeture de l'API
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

bool Jeu::MenuLancement(int h, int w,Mix_Music* music,SDL_Rect sizeTetris) {

	int numberChoice = 5;
	int sizeBetweenText = 80, xShift = 100;

	SDL_Window* pWindow = SDL_CreateWindow("TETRIS" , SDL_WINDOWPOS_CENTERED ,
				SDL_WINDOWPOS_CENTERED , w , h , SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);

	Tetris tetris(w,h, sizeTetris,renderer, true);
	Mix_Volume(-1,tetris.getvolume());

	SDL_Texture* startmenu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);

	SDL_SetRenderTarget(renderer, startmenu);
	SDL_SetRenderDrawColor(renderer,17,17,52,255);
	SDL_RenderFillRect(renderer, NULL);


	//Impression du menu
	tetris.printGenericMenu(startmenu,xShift,sizeBetweenText,false,numberChoice, "Tetris", "Jouer", "MultiJoueur",  "Leaderboard", "Parametres", "Quitter");

	//SDL_SetRenderTarget(renderer, startmenu);
	TTF_Font * policetetris = TTF_OpenFont("src/Tetris.ttf", 65);
	if(!policetetris){
		std::cout << TTF_GetError()<< std::endl;
	}

	TTF_Font * police = TTF_OpenFont("src/RetroGaming.ttf", 65);
	if(!police){
		std::cout << TTF_GetError()<< std::endl;
	}


	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, startmenu, NULL, NULL);

	SDL_RenderPresent(renderer);

	int choiceMenu = 0;
	//UODATE CETTE VARIABLE SI CHANGEMENT
	SDL_Rect cadre ={w/2-xShift-25, h/2- (numberChoice * sizeBetweenText)/2
			+ sizeBetweenText, 2*xShift+50, 40};

	bool quit_menu = false;
	ReturnCodeMenu gameState = ReturnCodeMenu::INIT;
	SDL_Event event;

	while (!quit_menu && SDL_WaitEvent(&event)){
		switch (event.type)
		{
		case SDL_QUIT:
			quit_menu = true;
			gameState = ReturnCodeMenu::QUIT_GAME;
			break;

		case SDL_KEYDOWN:
			switch( event.key.keysym.sym ){

			case SDLK_DOWN:
				tetris.UpDownCasesLoopMenu(0,1, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre);
				break;

			case SDLK_UP:
				tetris.UpDownCasesLoopMenu(0,0, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre);
				break;

			case SDLK_RETURN:
				//Play
				if (choiceMenu == 0){
					SDL_RenderClear(renderer);
					//on remet a la bonne taille
					//SDL_SetWindowSize(pWindow, w, h);
					tetris.init(music, false); //mettre a true si multiplayer
					SDL_RenderPresent(renderer);
					gameState = tetris.loop(music, false); //mettre a true si multiplayer
					quit_menu=true;
				}

				//multplayer
				else if (choiceMenu == 1){
					SDL_RenderClear(renderer);
					//on remet a la bonne taille
					//SDL_SetWindowSize(pWindow, w, h);
					tetris.init(music, true); //mettre a true si multiplayer
					SDL_RenderPresent(renderer);
					gameState = tetris.loop(music, true); //mettre a true si multiplayer
					quit_menu=true;
				}

				//settings
				else if (choiceMenu == 3){
					parametresmain(renderer, tetris);
					SDL_RenderCopy(renderer, startmenu, NULL, NULL);
					SDL_RenderPresent(renderer);
					choiceMenu = 0;
					cadre ={w/2-xShift-25, h/2- (numberChoice * sizeBetweenText)/2
							+ sizeBetweenText, 2*xShift+50, 40};
				}

				//Quit
				else if (choiceMenu == 4){
					gameState = ReturnCodeMenu::QUIT_GAME;
					quit_menu = true;
				}
				break;

			case SDLK_ESCAPE:
				quit_menu = true;
				break;

			default:
				break;
			}

		default :break;
		}
	}
	//TTF_CloseFont(police);
	FREE_TEXTURE(startmenu);
	FREE_RENDERER_AND_WINDOW(renderer, pWindow);
	TTF_CloseFont(police);
	TTF_CloseFont(policetetris);

	if (gameState == ReturnCodeMenu::QUIT_GAME ){
		return true;
	}
	return false;
}

void Jeu::parametresmain(SDL_Renderer* renderer, Tetris & tetris) {
	int numberChoice = 3, sizeBetweenText = 80, xShift = 100;
	int h= tetris.geth();
	int w= tetris.getw();
	SDL_Texture* parammenu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, tetris.getw(), tetris.geth());

	SDL_SetRenderTarget(renderer, parammenu);
	SDL_SetRenderDrawColor(renderer,17,17,52,255);
	SDL_RenderFillRect(renderer, NULL);

	tetris.printGenericMenu(parammenu,xShift,sizeBetweenText,1,numberChoice, "Tetris", "Audio", "Graphiques");
	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, parammenu, NULL, NULL);

	SDL_RenderPresent(renderer);


	int choiceMenu = 0;
	SDL_Rect cadre ={w/2-xShift-25, h/2- (numberChoice * sizeBetweenText)/2
			+ sizeBetweenText, 2*xShift+50, 40};
	bool quit_menu = false;
	SDL_Event event;

	while (!quit_menu && SDL_WaitEvent(&event)){
		switch (event.type)
		{
		case SDL_QUIT:
			quit_menu = true;
			break;

		case SDL_KEYDOWN:

			switch( event.key.keysym.sym ){

			case SDLK_DOWN:
				tetris.UpDownCasesLoopMenu(1,1, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre);
				break;

			case SDLK_UP:
				tetris.UpDownCasesLoopMenu(1,0, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre);
				break;

			case SDLK_RETURN:

				if (choiceMenu == 0){
					parametresaudio(renderer, tetris);
					SDL_RenderCopy(renderer, parammenu, NULL, NULL);
					SDL_RenderPresent(renderer);
					choiceMenu = 0;
					cadre ={w/2-xShift-25, h/2- (numberChoice * sizeBetweenText)/2
							+ sizeBetweenText, 2*xShift+50, 40};
				}

				if (choiceMenu == 1){
					parametresgraph(renderer, tetris);
					SDL_RenderCopy(renderer, parammenu, NULL, NULL);
					SDL_RenderPresent(renderer);
					choiceMenu = 0;
					cadre ={w/2-xShift-25, h/2- (numberChoice * sizeBetweenText)/2
							+ sizeBetweenText, 2*xShift+50, 40};
				}

				else if (choiceMenu == 2){
					SDL_SetRenderTarget(renderer, NULL);
					quit_menu = true;
				}
				break;

			case SDLK_ESCAPE:
				quit_menu = true;
				break;

			default:
				break;
			}

		default :break;
		}
	}
	FREE_TEXTURE(parammenu);
}

void Jeu::parametresaudio(SDL_Renderer* renderer, Tetris & tetris) {
	int numberChoice = 2;
	int sizeBetweenText = 80, xShift = 100;
	int h= tetris.geth();
	int w= tetris.getw();
	SDL_Texture* audiomenu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, tetris.getw(), tetris.geth());

	SDL_SetRenderTarget(renderer, audiomenu);
	SDL_SetRenderDrawColor(renderer,17,17,52,255);
	SDL_RenderFillRect(renderer, NULL);

	tetris.printGenericMenu(audiomenu,xShift,sizeBetweenText,1,numberChoice, "Tetris", "Volume audio");

	SDL_Rect cadre ={w/2-125, h/2, 250, 40};

	//on affiche un deuxième texte en dessous
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	//on met en noir le cadre qui aura été fait autour de Volume dans la fonction
	SDL_RenderDrawRect(renderer, &cadre);
	cadre.y += 50;
	cadre.h = 10;
	//On remplit le fond de la barre de volume
	SDL_RenderFillRect(renderer, &cadre);

	cadre.w = (tetris.getvolume()*250)/MIX_MAX_VOLUME;
	//on remplit la barre à la moitié
	SDL_SetRenderDrawColor(renderer,100,100,100,255);
	SDL_RenderFillRect(renderer, &cadre);

	//on encadre la barre de volume en blanc
	cadre.w = 250;
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderDrawRect(renderer, &cadre);

	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, audiomenu, NULL, NULL);

	SDL_RenderPresent(renderer);

	int choiceMenu = 0;
	//UODATE CETTE VARIABLE SI CHANGEMENT

	bool quit_menu = false;
	SDL_Event event;

	while (!quit_menu && SDL_WaitEvent(&event)){
		switch (event.type)
		{
		case SDL_QUIT:
			quit_menu = true;
			break;

		case SDL_KEYDOWN:

			switch( event.key.keysym.sym ){

			case SDLK_DOWN:
				tetris.UpDownCasesLoopMenu(1,1, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre, true);
				break;

			case SDLK_UP:
				tetris.UpDownCasesLoopMenu(1,0, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre, true);
				break;

			case SDLK_LEFT:
				if(choiceMenu==0) {
					Mix_VolumeMusic(tetris.downvolume());
					SDL_SetRenderDrawColor(renderer,0,0,0,255);
					cadre = { w/2-100-25, h/2+50, 200+50, 10};
					SDL_RenderFillRect(renderer, &cadre);
					cadre.w = (tetris.getvolume()*250)/MIX_MAX_VOLUME;
					SDL_SetRenderDrawColor(renderer,100,100,100,255);
					SDL_RenderFillRect(renderer, &cadre);
					SDL_RenderPresent(renderer);
				}
				break;

			case SDLK_RIGHT:
				if(choiceMenu==0) {
					Mix_VolumeMusic(tetris.upvolume());
					SDL_SetRenderDrawColor(renderer,0,0,0,255);
					cadre = { w/2-100-25, h/2+50, 200+50, 10};
					SDL_RenderFillRect(renderer, &cadre);
					cadre.w = (tetris.getvolume()*250)/MIX_MAX_VOLUME;
					SDL_SetRenderDrawColor(renderer,100,100,100,255);
					SDL_RenderFillRect(renderer, &cadre);
					SDL_RenderPresent(renderer);
				}
				break;

			case SDLK_RETURN:

				if (choiceMenu == 1){
					quit_menu = true;
				}
				break;

			case SDLK_ESCAPE:
				quit_menu = true;
				break;

			default:
				break;
			}

		default :break;
		}
	}
	FREE_TEXTURE(audiomenu);
}

void Jeu::DrawCheckboxes(SDL_Renderer* renderer, SDL_Texture * graphmenu,Tetris &tetris) {
	SDL_SetRenderTarget(renderer,graphmenu);
	int h= tetris.geth();
	int w= tetris.getw();
	int sizeBetweenText = 80;
	SDL_Rect checkbox = { w/2-15, h/2 + sizeBetweenText + 15, 30, 30};
	unsigned int modes[3] = {WHITE_LINED,MILIEU,EYES};
	for(int i = 0; i< 3; i++) {
		SDL_SetRenderDrawColor(renderer,150,150,150,255);
		SDL_RenderFillRect(renderer, &checkbox);
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		SDL_RenderDrawRect(renderer, &checkbox);
		if(tetris.getopt()&modes[i]) {
			SDL_RenderDrawLine(renderer,checkbox.x,checkbox.y, checkbox.x+checkbox.w, checkbox.y+checkbox.h);
			SDL_RenderDrawLine(renderer,checkbox.x, checkbox.y+checkbox.h, checkbox.x+checkbox.w,checkbox.y);
		}
		if(i==0) checkbox.x+= sizeBetweenText+250;
		else checkbox.x-= 2*(sizeBetweenText+250);
	}
	SDL_SetRenderTarget(renderer,NULL);
	SDL_RenderCopy(renderer, graphmenu, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_SetRenderTarget(renderer, graphmenu);
}

void Jeu::fillblankoptions(SDL_Rect * rect, unsigned int options, SDL_Renderer * renderer,SDL_Texture * graphmenu, SDL_Texture * blankmenu) {
	SDL_SetRenderTarget(renderer,blankmenu);
	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	if(options&CLASSIC) {
		SDL_SetRenderDrawColor(renderer,175,175,135,255);
	}
	else if(options&ACCESS) {
		SDL_SetRenderDrawColor(renderer,10,10,10,255);
	}
	else if(options&PASTEL) {
		SDL_SetRenderDrawColor(renderer,212,255,254,255);
	}
	SDL_RenderFillRect(renderer,rect);
	SDL_SetRenderDrawColor(renderer,230,230,230,255);
	SDL_RenderDrawRect(renderer, rect);

	SDL_SetRenderTarget(renderer,graphmenu);
	SDL_RenderCopy(renderer, blankmenu, rect, rect);
	SDL_SetRenderTarget(renderer,NULL);
	SDL_RenderCopy(renderer, graphmenu, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_SetRenderTarget(renderer, graphmenu);
}

void Jeu::drawprev(Piece * prev, unsigned int options, SDL_Rect* rect,
			SDL_Renderer * renderer, SDL_Texture * blankmenu, SDL_Texture * graphmenu) {
	prev->setopt(options);
	fillblankoptions(rect, options, renderer, graphmenu, blankmenu);
	prev->draw(renderer, blankmenu, graphmenu);
	SDL_RenderPresent(renderer);
}

void Jeu::parametresgraph(SDL_Renderer* renderer, Tetris & tetris) {
	int numberChoice = 3;
	int sizeBetweenText = 80, xShift = 100;
	int h= tetris.geth();
	int w= tetris.getw();
	SDL_Texture* graphmenu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_Texture* blankmenu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);

	SDL_SetRenderTarget(renderer, graphmenu);
	SDL_SetRenderDrawColor(renderer,17,17,52,255);
	SDL_RenderFillRect(renderer, NULL);

	SDL_SetRenderTarget(renderer,blankmenu);
	SDL_RenderCopy(renderer, graphmenu, NULL, NULL);
	SDL_SetRenderTarget(renderer, graphmenu);

	tetris.printGenericMenu(graphmenu,xShift,sizeBetweenText,1,numberChoice, "Parametres Graphiques", "Mode (derouler)", "Lignes Blanches");
	const char * str1= "Centre";
	const char * str2= "Yeux";
	tetris.addmenuoptions(graphmenu,xShift,sizeBetweenText,numberChoice, 2, 2, str1, str2);
	DrawCheckboxes(renderer, graphmenu, tetris);



	SDL_Rect cadre = { w/2-xShift-25 , h/2 - (numberChoice * sizeBetweenText)/2+sizeBetweenText, 2*xShift+50, 40};

	//SDL_Rect cadreprev = {w/2-75-sizeBetweenText-3*xShift, h/2+2*sizeBetweenText, 2*(2*xShift+sizeBetweenText), h/4};


	LTetri * prev = new LTetri(tetris.getopt());
	int factor = prev->locTetris.x/BLOCSX;
	SDL_Rect cadreprev = {w/2-75-sizeBetweenText-3*xShift, h/2+2*sizeBetweenText, 3*factor, 4*factor};

	prev->update();
	prev->placeprev((cadreprev.x-prev->locTetris.x)/factor,(cadreprev.y-prev->locTetris.y)/factor);
	drawprev(prev, tetris.getopt(), &cadreprev, renderer, blankmenu, graphmenu);
	prev->affiche_coord(1,1);

	//SDL_RenderCopy(renderer,blankmenu, NULL, NULL);
	//SDL_RenderPresent(renderer);
	//on affiche les deux autres modes

	int choiceMenu = 0;
	int choiceX = 0;

	bool quit_menu = false;
	SDL_Event event;

	SDL_SetRenderTarget(renderer,NULL);
	SDL_RenderCopy(renderer, graphmenu, NULL, NULL);
	SDL_SetRenderTarget(renderer, graphmenu);

	while (!quit_menu && SDL_WaitEvent(&event)){
		switch (event.type)
		{
		case SDL_QUIT:
			quit_menu = true;
			break;

		case SDL_KEYDOWN:

			switch( event.key.keysym.sym ){

			case SDLK_DOWN:
				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);
				if(choiceX!=0) {
					cadre.x = w/2-xShift-25;
					choiceX=0;
				}
				tetris.UpDownCasesLoopMenu(1,1, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre);
				break;

			case SDLK_UP:
				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);
				if(choiceX!=0) {
					cadre.x = w/2-xShift-25;
					choiceX=0;
				}
				tetris.UpDownCasesLoopMenu(1,0, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre);
				break;

			case SDLK_LEFT:
				if(choiceMenu==1) {
					SDL_SetRenderDrawColor(renderer,0,0,0,255);
					SDL_RenderDrawRect(renderer, &cadre);
					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					if(choiceX>-1) {
						choiceX--;
						cadre.x -= sizeBetweenText+2*xShift+50;
					}
					SDL_RenderDrawRect(renderer, &cadre);
					SDL_RenderPresent(renderer);
				}
				break;

			case SDLK_RIGHT:
				if(choiceMenu==1) {
					SDL_SetRenderDrawColor(renderer,0,0,0,255);
					SDL_RenderDrawRect(renderer, &cadre);
					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					if(choiceX<1) {
						choiceX++;
						cadre.x += sizeBetweenText+2*xShift+50;
					}
					SDL_RenderDrawRect(renderer, &cadre);
					SDL_RenderPresent(renderer);
				}
				break;

			case SDLK_RETURN:
				if(choiceMenu==0) {
					tetris.minimenu(graphmenu, &cadre);
					cadre = { w/2-xShift-25, h/2 - (numberChoice * sizeBetweenText)/2+sizeBetweenText, 2*xShift+50, 40};
					SDL_SetRenderDrawColor(renderer,0,0,0,255);
					SDL_RenderDrawRect(renderer, &cadre);
					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					drawprev(prev, tetris.getopt(), &cadreprev, renderer, blankmenu, graphmenu);
					DrawCheckboxes(renderer, graphmenu, tetris);
				}
				else if(choiceMenu==1) {
					if(choiceX==-1) tetris.setoption(EYES);
					else if(choiceX == 0) tetris.setoption(WHITE_LINED);
					else tetris.setoption(MILIEU);
					drawprev(prev, tetris.getopt(), &cadreprev, renderer, blankmenu, graphmenu);
					DrawCheckboxes(renderer,graphmenu, tetris);
				}

				else if (choiceMenu == 2){
					quit_menu = true;
				}
				break;

			case SDLK_ESCAPE:
				quit_menu = true;
				break;

			default:
				break;
			}

		default :break;
		}
		SDL_SetRenderTarget(renderer,NULL);
		SDL_RenderCopy(renderer, graphmenu, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_SetRenderTarget(renderer, graphmenu);
	}
	FREE_TEXTURE(graphmenu);
}

int main(int argc, char** argv)
{
	int h= SIZE_BLOC*BLOCSY;
	int w= SIZE_BLOC*BLOCSX + 500;
	SDL_Rect sizeTetris;
	sizeTetris.w=310;
	sizeTetris.x=0;
	sizeTetris.y=0;
	Jeu jeu;
	jeu.startTetris(h,w, sizeTetris);
}
