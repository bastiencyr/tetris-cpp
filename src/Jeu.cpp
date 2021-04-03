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

	SDL_Window* pWindow = SDL_CreateWindow("TETRIS" , SDL_WINDOWPOS_CENTERED ,
				SDL_WINDOWPOS_CENTERED , w , h , SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);

	Tetris tetris(w,h, sizeTetris,renderer, true);
	Mix_Volume(-1,tetris.getvolume());

	SDL_Texture* startmenu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);

	SDL_SetRenderTarget(renderer, startmenu);
	TTF_Font * policetetris = TTF_OpenFont("src/Tetris.ttf", 65);
	if(!policetetris){
		std::cout << TTF_GetError()<< std::endl;
	}

	TTF_Font * police = TTF_OpenFont("src/RetroGaming.ttf", 65);
	if(!police){
		std::cout << TTF_GetError()<< std::endl;
	}

	SDL_Color textColor = {255, 255, 255};
	SDL_Surface * text_surface = TTF_RenderText_Blended(policetetris,"Tetris", textColor);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	//on remplit le fond
	SDL_SetRenderTarget(renderer, text_texture);
	SDL_SetRenderDrawColor(renderer,17,17,52,255);
	SDL_RenderFillRect(renderer, NULL);

	//SDL_SetRenderTarget(renderer, startmenu);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_Rect dstrect = { w/2-100, h/4, 200, 40 };
	SDL_Rect cadrect = { w/2-100-25, h/4, 200+50, 40};


	//SDL_SetRenderTarget(renderer, menu);
	//on copie le text sur le menu
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	//premier texte hors titre
	//on affiche un deuxième texte en dessous

	text_surface = TTF_RenderText_Blended(police,"Jouer", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.y = h/2;
	cadrect.y = h/2;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);


	//on affiche un deuxième texte en dessous
	text_surface = TTF_RenderText_Blended(police,"Leaderboard", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.y += 80;
	cadrect.y += 80;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);

	//troisième texte
	text_surface = TTF_RenderText_Blended(police,"Parametres", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.y += 80;
	cadrect.y += 80;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);

	//quatrième texte
	text_surface = TTF_RenderText_Blended(police,"Quitter", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.x += 40;
	dstrect.y += 80;
	dstrect.w = 120;
	cadrect.y += 80;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);


	//on dessine le cadre du texte
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_Rect cadre ={w/2-100-25, h/2, 250, 40};
	SDL_RenderDrawRect(renderer, &cadre);



	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, startmenu, NULL, NULL);
	SDL_RenderDrawLine(renderer,w/2-100-25,h/4-10,w/2+125,h/4-10);
	SDL_RenderDrawLine(renderer,w/2-100-25,h/4+50,w/2+125,h/4+50);

	SDL_RenderPresent(renderer);

	//free(text_surface);
	//free(text_texture);

	int choiceMenu = 0;
	//UODATE CETTE VARIABLE SI CHANGEMENT
	int numberChoice = 4;

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
				choiceMenu+=1;
				choiceMenu = choiceMenu % numberChoice ;
				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				cadre.y =h/2 +80 *choiceMenu ;
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);

				break;

			case SDLK_UP:
				choiceMenu-=1 - numberChoice;
				choiceMenu = choiceMenu % numberChoice ;

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				cadre.y = h/2 + 80*choiceMenu;
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);
				break;

			case SDLK_RETURN:


				//lancer le jeu
				if (choiceMenu == 0){
					tetris.init(music, true);
					SDL_RenderPresent(renderer);
					tetris.loop(music);
					quit_menu=true;
				}

				else if (choiceMenu == 2){
					parametresmain(renderer, tetris, policetetris,police);
					SDL_RenderCopy(renderer, startmenu, NULL, NULL);
					SDL_RenderPresent(renderer);
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
	TTF_CloseFont(police);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	return tetris.getquit();
}

void Jeu::parametresmain(SDL_Renderer* renderer, Tetris tetris, TTF_Font * P1, TTF_Font * P2) {
	SDL_Color textColor = {255, 255, 255};
	int h= tetris.geth();
	int w= tetris.getw();
	SDL_Texture* startmenu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetRenderTarget(renderer, startmenu);
	SDL_Surface * text_surface = TTF_RenderText_Blended(P1,"Parametres", textColor);

	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);


	//on remplit le fond
	SDL_SetRenderTarget(renderer, text_texture);
	SDL_SetRenderDrawColor(renderer,17,17,52,255);
	SDL_RenderFillRect(renderer, NULL);

	SDL_SetRenderTarget(renderer, startmenu);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_Rect dstrect = { w/2-100, h/4, 200, 40 };
	SDL_Rect cadrect = { w/2-100-25, h/4, 200+50, 40};


	//SDL_SetRenderTarget(renderer, menu);
	//on copie le text sur le menu
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	//premier texte hors titre
	//on affiche un deuxième texte en dessous

	text_surface = TTF_RenderText_Blended(P2,"Audio", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.y = h/2;
	cadrect.y = h/2;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);


	//on affiche un deuxième texte en dessous
	text_surface = TTF_RenderText_Blended(P2,"Graphiques", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.y += 80;
	cadrect.y += 80;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);

	//on dessine le bouton retour
	text_surface = TTF_RenderText_Blended(P2,"Retour", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.x = 3*w/4-20;
	cadrect.x = 3*w/4-45;
	dstrect.y = 7*h/8;
	cadrect.y = 7*h/8;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);

	//on dessine le cadre du texte
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_Rect cadre ={w/2-100-25, h/2, 250, 40};
	SDL_RenderDrawRect(renderer, &cadre);



	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, startmenu, NULL, NULL);
	SDL_RenderDrawLine(renderer,w/2-100-25,h/4-10,w/2+125,h/4-10);
	SDL_RenderDrawLine(renderer,w/2-100-25,h/4+50,w/2+125,h/4+50);

	SDL_RenderPresent(renderer);

	//free(text_surface);
	//free(text_texture);

	int choiceMenu = 0;
	//UODATE CETTE VARIABLE SI CHANGEMENT
	int numberChoice = 3;

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
				choiceMenu+=1;
				choiceMenu = choiceMenu % numberChoice ;
				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				if(choiceMenu < 2)  {
					cadre.x = w/2-100-25;
					cadre.y =h/2 +80 *choiceMenu ;}
				else {
					cadre.y =7*h/8 ;
					cadre.x = 3*w/4-45;
				}
				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);

				break;

			case SDLK_UP:
				choiceMenu-=1 - numberChoice;
				choiceMenu = choiceMenu % numberChoice ;

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				if(choiceMenu < 2)  {
					cadre.x = w/2-100-25;
					cadre.y =h/2 +80 *choiceMenu ;}
				else {
					cadre.y =7*h/8 ;
					cadre.x = 3*w/4-45;
				}
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);
				break;

			case SDLK_RETURN:


				//lancer le jeu
				if (choiceMenu == 0){
					parametresaudio(renderer, tetris, P1,P2);
					SDL_RenderCopy(renderer, startmenu, NULL, NULL);
					SDL_RenderPresent(renderer);
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
	}

}

void Jeu::parametresaudio(SDL_Renderer* renderer, Tetris tetris, TTF_Font * P1, TTF_Font * P2) {
	SDL_Color textColor = {255, 255, 255};
	int h= tetris.geth();
	int w= tetris.getw();
	SDL_Texture* startmenu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetRenderTarget(renderer, startmenu);
	SDL_Surface * text_surface = TTF_RenderText_Blended(P1,"Parametres Audio", textColor);

	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);


	//on remplit le fond
	SDL_SetRenderTarget(renderer, text_texture);
	SDL_SetRenderDrawColor(renderer,17,17,52,255);
	SDL_RenderFillRect(renderer, NULL);

	SDL_SetRenderTarget(renderer, startmenu);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_Rect dstrect = { w/2-100, h/4, 200, 40 };
	SDL_Rect cadrect = { w/2-100-25, h/4, 200+50, 40};


	//SDL_SetRenderTarget(renderer, menu);
	//on copie le text sur le menu
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	//premier texte hors titre
	//on affiche un deuxième texte en dessous

	text_surface = TTF_RenderText_Blended(P2,"Volume Sonore", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.y = h/2;
	cadrect.y = h/2;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);


	//on affiche un deuxième texte en dessous
	cadrect.y += 50;
	cadrect.h = 10;
	SDL_RenderFillRect(renderer, &cadrect);
	cadrect.w = (tetris.getvolume()*250)/MIX_MAX_VOLUME;
	cadrect.h = 10;
	SDL_SetRenderDrawColor(renderer,100,100,100,255);
	SDL_RenderFillRect(renderer, &cadrect);
	//SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);

	//on dessine le bouton retour
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	text_surface = TTF_RenderText_Blended(P2,"Retour", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.x = 3*w/4-20;
	cadrect.x = 3*w/4-45;
	dstrect.y = 7*h/8;
	cadrect.w = 250;
	cadrect.h = 40;
	cadrect.y = 7*h/8;
	SDL_RenderFillRect(renderer, &cadrect);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);

	//on dessine le cadre du texte
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_Rect cadre ={w/2-125, h/2+50, 250, 10};
	SDL_RenderDrawRect(renderer, &cadre);



	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, startmenu, NULL, NULL);
	SDL_RenderDrawLine(renderer,w/2-100-25,h/4-10,w/2+125,h/4-10);
	SDL_RenderDrawLine(renderer,w/2-100-25,h/4+50,w/2+125,h/4+50);

	SDL_RenderPresent(renderer);

	//free(text_surface);
	//free(text_texture);

	int choiceMenu = 0;
	//UODATE CETTE VARIABLE SI CHANGEMENT
	int numberChoice = 2;

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
				choiceMenu+=1;
				choiceMenu = choiceMenu % numberChoice ;
				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				if(choiceMenu == 0)  {
					cadre.x = w/2-100-25;
					cadre.y =h/2+50;
					//cadre.w = 250;
					cadre.h = 10;}
				else {
					cadre.y =7*h/8 ;
					cadre.x = 3*w/4-45;
					cadre.h=40;
				}
				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);

				break;

			case SDLK_UP:
				choiceMenu-=1 - numberChoice;
				choiceMenu = choiceMenu % numberChoice ;

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				if(choiceMenu == 0)  {
					cadre.x = w/2-100-25;
					cadre.y =h/2+50;
					//cadre.w = 250;
					cadre.h = 10;}
				else {
					cadre.y =7*h/8 ;
					cadre.x = 3*w/4-45;
					cadre.h=40;
				}
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);
				break;

			case SDLK_LEFT:
				if(choiceMenu==0) {
					Mix_VolumeMusic(tetris.downvolume());
					SDL_SetRenderDrawColor(renderer,0,0,0,255);
					cadrect = { w/2-100-25, h/2+50, 200+50, 10};
					SDL_RenderFillRect(renderer, &cadrect);
					cadrect.w = (tetris.getvolume()*250)/MIX_MAX_VOLUME;
					SDL_SetRenderDrawColor(renderer,100,100,100,255);
					SDL_RenderFillRect(renderer, &cadrect);
					SDL_RenderPresent(renderer);
				}
				break;

			case SDLK_RIGHT:
				if(choiceMenu==0) {
					Mix_VolumeMusic(tetris.upvolume());
					SDL_SetRenderDrawColor(renderer,0,0,0,255);
					cadrect = { w/2-100-25, h/2+50, 200+50, 10};
					SDL_RenderFillRect(renderer, &cadrect);
					cadrect.w = (tetris.getvolume()*250)/MIX_MAX_VOLUME;
					SDL_SetRenderDrawColor(renderer,100,100,100,255);
					SDL_RenderFillRect(renderer, &cadrect);
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

}

int main(int argc, char** argv)
{
	int h=SIZE_BLOC*BLOCSY;
	int w=SIZE_BLOC*BLOCSX + 500;
	SDL_Rect sizeTetris;
	sizeTetris.w=310;
	sizeTetris.x=0;
	sizeTetris.y=0;
	Jeu jeu;
	jeu.startTetris(h,w, sizeTetris, true);
}
