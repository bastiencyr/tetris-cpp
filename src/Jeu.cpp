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
#define SIZE_BLOC 35
#define BLOCSX 10
#define BLOCSY 20



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
		//on optimise l'espace
		int minFactor = dm.w / (3*BLOCSX) < dm.h / BLOCSY ? dm.w / (3*BLOCSX) : dm.h / BLOCSY;
		w = 3*BLOCSX * minFactor;
		h = BLOCSY * minFactor;
		
		w = (int)((float)(w)*(4./5.));
		h = (int)((float)(h)*(4./5.));

		sizeTetris.w = (w/3)/(w/3/5) * (w/3/5) ;
		sizeTetris.h = sizeTetris.w * 2;
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
   	Mix_CloseAudio(); //Fermeture de l'API
	TTF_Quit();
	SDL_Quit();
}

bool Jeu::MenuLancement(int h, int w,Mix_Music* music,SDL_Rect sizeTetris) {

	int numberChoice = 4;
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
	tetris.printGenericMenu(startmenu,xShift,sizeBetweenText,false,numberChoice, "Tetris", "Jouer", "Leaderboard", "Parametres", "Quitter");

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
				tetris.UpDownCasesLoopMenu(0,1, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre);
				break;

			case SDLK_UP:
				tetris.UpDownCasesLoopMenu(0,0, choiceMenu, numberChoice, sizeBetweenText, xShift, cadre);
				break;

			case SDLK_RETURN:
				//lancer le jeu
				if (choiceMenu == 0){
					SDL_RenderClear(renderer);
					//on remet a la bonne taille
					//SDL_SetWindowSize(pWindow, w, h);
					tetris.init(music, false); //mettre a true si multiplayer
					SDL_RenderPresent(renderer);
					tetris.loop(music, false); //mettre a true si multiplayer
					quit_menu=true;
				}
				else if (choiceMenu == 2){
					parametresmain(renderer, tetris, policetetris,police);
					SDL_RenderCopy(renderer, startmenu, NULL, NULL);
					SDL_RenderPresent(renderer);
					choiceMenu = 0;
					cadre ={w/2-xShift-25, h/2- (numberChoice * sizeBetweenText)/2
							+ sizeBetweenText, 2*xShift+50, 40};
				}
				else if (choiceMenu == 3){
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
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	return tetris.getquit();
}

void Jeu::parametresmain(SDL_Renderer* renderer, Tetris & tetris, TTF_Font * P1, TTF_Font * P2) {
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
					parametresaudio(renderer, tetris, P1,P2);
					SDL_RenderCopy(renderer, parammenu, NULL, NULL);
					SDL_RenderPresent(renderer);
					choiceMenu = 0;
					SDL_Rect cadre ={w/2-xShift-25, h/2- (numberChoice * sizeBetweenText)/2
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
	SDL_DestroyTexture(parammenu);
}

void Jeu::parametresaudio(SDL_Renderer* renderer, Tetris & tetris, TTF_Font * P1, TTF_Font * P2) {
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
	SDL_DestroyTexture(audiomenu);
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
