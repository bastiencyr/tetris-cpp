/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <SDL.h>
#include "SDL_mixer.h"
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
	Mix_Music* music = Mix_LoadMUS("bin/tetris.mp3");

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

	/*SDL_AudioSpec obtained;
	SDL_AudioSpec desired;

	desired.freq=48000;
	desired.format = AUDIO_F32SYS;
	desired.channels= 2;
	desired.samples = 4096;
	desired.callback = [](void* param, Uint8* stream, int len)
	{
    // Envoyez les données dans notre buffer...
		int samples = len / sizeof(float); // 4096

	    for (auto i = 0; i < samples; i++)
	    {
	        reinterpret_cast<float*>(stream)[i] = 0.5 * SDL_sinf(2 * M_PI * i / 1000);
	    }
	};

	SDL_AudioDeviceID dev = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

	SDL_PauseAudioDevice(dev, SDL_FALSE);

    for (;;); // boucle infinie*/

	Tetris tetris(w,h, sizeTetris);
	tetris.init();
	SDL_RenderPresent(tetris.get_renderer());
	//SDL_Delay(10000);
	tetris.loop();

	Mix_FreeMusic(music); //Libération de la musique
   	Mix_CloseAudio(); //Fermeture de l'API
	SDL_Quit();
}

int main(int argc, char** argv)
{
	int h=SIZE_BLOC*BLOCSY;
	int w=SIZE_BLOC*BLOCSX;
	SDL_Rect sizeTetris;
	sizeTetris.w=250;
	sizeTetris.h=h;
	sizeTetris.x=0;
	sizeTetris.y=20;
	Jeu jeu;
	jeu.startTetris(h,w, sizeTetris);
}
