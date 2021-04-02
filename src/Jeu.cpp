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



void Jeu::startTetris(int h,int w, SDL_Rect sizeTetris){

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
	Mix_Music* music = Mix_LoadMUS("sfx/tetris.mp3");

	if (music == nullptr)
	{
	    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
	    Mix_CloseAudio();
	    SDL_Quit();
	    return;
	}

	if(Mix_PlayMusic(music, -1)==-1) {
    	printf("Mix_PlayMusic: %s\n", Mix_GetError());
		// well, there's no music, but most games don't break without music...
	}

	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	Jeu::MenuLancement(h,w,music, sizeTetris);

//	Mix_FreeMusic(music); //Libération de la musique
   	Mix_CloseAudio(); //Fermeture de l'API
	TTF_Quit();
	SDL_Quit();
}

void Jeu::MenuLancement(int h, int w,Mix_Music* music,SDL_Rect sizeTetris) {
	SDL_Window* pWindow = SDL_CreateWindow("Une fenetre SDL" , SDL_WINDOWPOS_CENTERED ,
				SDL_WINDOWPOS_CENTERED , w , h , SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);
	Tetris tetris(w,h, sizeTetris,renderer);
	//SDL_Renderer* renderer= tetris.get_renderer();
	SDL_Texture* menu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);


	TTF_Font *police = TTF_OpenFont("src/Tetris.ttf", 65);
	if(!police){
		std::cout << TTF_GetError()<< std::endl;
	}

	SDL_SetRenderTarget(renderer, menu);
	SDL_Color textColor = {255, 255, 255};
	SDL_Surface * text_surface = TTF_RenderText_Blended(police,"Tetris", textColor);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_SetRenderTarget(renderer, text_texture);
	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	SDL_RenderFillRect(renderer, NULL);
	SDL_SetRenderTarget(renderer, menu);

	SDL_Rect dstrect = { sizeTetris.w/2-50, sizeTetris.h/4, 200, 40 };
	SDL_Rect cadrect = { sizeTetris.w/2-50-25, sizeTetris.h/4, 200+50, 40};

	//on copie le text sur le menu
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	//premier texte hors titre
	//on affiche un deuxième texte en dessous

	text_surface = TTF_RenderText_Blended(police,"Jouer", textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	dstrect.y = sizeTetris.h/2;
	cadrect.y = sizeTetris.h/2;
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
	text_surface = TTF_RenderText_Blended(police,"Paramètres", textColor);
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
	SDL_Rect cadre ={sizeTetris.w/2-50-25, sizeTetris.h/2, 250, 40};
	SDL_RenderDrawRect(renderer, &cadre);



	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, menu, NULL, NULL);

	SDL_RenderPresent(renderer);

	TTF_CloseFont(police);
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
				cadre.y =sizeTetris.h/2 +80 *choiceMenu ;
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);

				break;

			case SDLK_UP:
				choiceMenu-=1 - numberChoice;
				choiceMenu = choiceMenu % numberChoice ;

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				cadre.y = sizeTetris.h/2 + 80*choiceMenu;
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);
				break;

			case SDLK_RETURN:


				//lancer le jeu
				if (choiceMenu == 0){
					tetris.init(music);
					SDL_RenderPresent(tetris.get_renderer());
					tetris.loop(music);
					quit_menu=true;
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
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
}

int main(int argc, char** argv)
{
	int h=SIZE_BLOC*BLOCSY+200;
	int w=SIZE_BLOC*BLOCSX + 200;
	SDL_Rect sizeTetris;
	sizeTetris.w=310;
	sizeTetris.x=30;
	sizeTetris.y=30;
	Jeu jeu;
	jeu.startTetris(h,w, sizeTetris);
}
