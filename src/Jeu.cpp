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

	Tetris tetris(w,h, sizeTetris);
	tetris.init(music);
	SDL_RenderPresent(tetris.get_renderer());
	tetris.loop(music);

	Mix_FreeMusic(music); //Libération de la musique
   	Mix_CloseAudio(); //Fermeture de l'API
	TTF_Quit();
	SDL_Quit();
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
