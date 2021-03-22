/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cassert>
#include "../include/tetris.hpp"
#define SIZE_BLOC 35
#define BLOCSX 10
#define BLOCSY 20

int main(int argc, char** argv)
{	
	int h=SIZE_BLOC*BLOCSY;
	int w=SIZE_BLOC*BLOCSX;

	if(SDL_VideoInit(NULL) < 0) // Initialisation de la SDL
	{
		printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
		return EXIT_FAILURE;
	}

	Tetris tetris(w,h);
	tetris.init();
	SDL_RenderPresent(tetris.get_renderer());
	tetris.loop();

	SDL_Quit();
}