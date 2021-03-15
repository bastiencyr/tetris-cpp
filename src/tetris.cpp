/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <SDL.h>
#include <string>
#include <iostream>
#include <cassert>
#include "../include/tetris.hpp"
Tetris::Tetris(int w, int h) {
	this->w=w;
	this->h=h;
	pWindow = SDL_CreateWindow("Une fenetre SDL" , SDL_WINDOWPOS_CENTERED ,
			SDL_WINDOWPOS_CENTERED , w , h , SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);
	//renderer2 = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED |
	//SDL_RENDERER_PRESENTVSYNC);
	timer=0;
	
	winSurf=SDL_GetWindowSurface(pWindow);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, 200, 200);
	
}

void Tetris::init(){
	SDL_Point ligne_depart,ligne_arrivee; // Déclaration du point de départ et du point d'arrivée d'une ligne
	
	//on colorie le fond. La fonction renderDrawColor permet d'initialiser une
	//couleur. Ainsi, toute fonction suivante utilisant une couleur utilisera
	//implicitement cette couleur
	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);
	//Le fond est maintenant gris
	
	// À présent, occupons nous des lignes
	// On ne peut pas utiliser la fonction SDL_RenderDrawLines
	// car celle-ci ne permet pas de créer des lignes indépendantes comme nous voulons le faire mais des chemins
	
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	
	// Lignes horizontales
	ligne_depart.x = 0;
	ligne_arrivee.x = h;
	ligne_depart.y = 0;
	
	while(ligne_depart.y<h)
	{
		ligne_depart.y += 40;
		ligne_arrivee.y = ligne_depart.y;
		SDL_RenderDrawLine(renderer,ligne_depart.x, ligne_depart.y,ligne_arrivee.x,ligne_arrivee.y);
	}
	
	// Lignes verticales
	ligne_depart.x = 0;
	ligne_depart.y = 0;
	ligne_arrivee.y = h;
	
	while(ligne_depart.x<h){
		ligne_depart.x += 40;
		ligne_arrivee.x = ligne_depart.x;
		SDL_RenderDrawLine(renderer,ligne_depart.x, ligne_depart.y,ligne_arrivee.x,ligne_arrivee.y);
	}
	
}


Tetris::~Tetris() {
}

void Tetris::loop()
{
	Uint64 prev, now = SDL_GetPerformanceCounter(); // timers
	double delta_t;  // durée frame en ms
	
	bool quit = false;
	double t=0;
	while (!quit)
	{
		SDL_Event event;
		
		while (!quit && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			
			case SDL_KEYDOWN:
               /* Check the SDLKey values and move change the coords */
				switch( event.key.keysym.sym ){
					case SDLK_RIGHT:
						piece.right();
						piece.draw(renderer);
						break;
						
					case SDLK_LEFT:
						piece.left();
						piece.draw(renderer);
						break;
						
					case SDLK_DOWN:
						piece.down();
						piece.draw(renderer);
						break;
				}
				
			default: break;
			}
		}
		
		const Uint8* state = SDL_GetKeyboardState(NULL);
		quit |= (state[SDL_SCANCODE_ESCAPE]!=0);
		
		prev = now;
		now = SDL_GetPerformanceCounter();
		delta_t = static_cast<double>((now - prev)/
				static_cast<float>(SDL_GetPerformanceFrequency()));
		
		t+=delta_t;
		if(floor(t)>=1) {
			piece.down();
			piece.draw(renderer);
			t=0;
		}
	}
	
}

int main(int argc, char** argv)
{
	int h=640;
	int w=400;
	
	if(SDL_VideoInit(NULL) < 0) // Initialisation de la SDL
	{
		printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
		return EXIT_FAILURE;
	}
	
	Tetris tetris(w,h);
	tetris.init();
	SDL_RenderPresent(tetris.renderer);
	tetris.loop();
	
	SDL_DestroyRenderer(tetris.renderer);
	SDL_DestroyWindow(tetris.pWindow);
	SDL_Quit();
}
