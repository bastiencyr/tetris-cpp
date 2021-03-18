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
#define SIZE_BLOC 35
#define BLOCSX 10
#define BLOCSY 20


Tetris::Tetris(int w, int h) : piece(w) {
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
			SDL_TEXTUREACCESS_TARGET, w, h);

	//mat[BLOCSX][BLOCSY];
	for(int i = 0; i < BLOCSX; i++) {
		for(int j = 0; j < BLOCSY; j++) {
			mat[i][j] = false;
		}
	}
	
	std::cout << "L'état initial de la matrice est : " << std::endl;
	for(int j = 0; j< BLOCSY; j++) {
		for(int i = 0; i< BLOCSX; i++) {
			if (mat[i][j]==true)
				std::cout << "--0--" ;
			else
				std::cout << "--1--" ;
		}
		std::cout << std::endl;
	}

	piece.affiche_coord(1,1);
	//piece.draw(renderer,texture, SIZE_BLOC);
}

void Tetris::init(){
	SDL_Point ligne_depart,ligne_arrivee; // Déclaration du point de départ et du point d'arrivée d'une ligne

	//on colorie le fond. La fonction renderDrawColor permet d'initialiser une
	//couleur. Ainsi, toute fonction suivante utilisant une couleur utilisera
	//implicitement cette couleur
	SDL_SetRenderTarget(renderer, texture);

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
		ligne_depart.y += SIZE_BLOC;
		ligne_arrivee.y = ligne_depart.y;
		SDL_RenderDrawLine(renderer,ligne_depart.x, ligne_depart.y,ligne_arrivee.x,ligne_arrivee.y);
	}

	// Lignes verticales
	ligne_depart.x = 0;
	ligne_depart.y = 0;
	ligne_arrivee.y = h;

	while(ligne_depart.x<h){
		ligne_depart.x += SIZE_BLOC;
		ligne_arrivee.x = ligne_depart.x;
		SDL_RenderDrawLine(renderer,ligne_depart.x, ligne_depart.y,ligne_arrivee.x,ligne_arrivee.y);
	}

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
}


Tetris::~Tetris() {
}

void Tetris::loop()
{
	Uint64 prev, now = SDL_GetPerformanceCounter(); // timers
	double delta_t;  // durée frame en ms

	bool quit = false;
	bool cont = true;
	double t=0;
	while (!quit)
	{
		if(!cont) {
			std::cout << "Nouvelle pièce en haut" << std::endl;
			Piece NouvPiece(w);
			piece = NouvPiece;
			cont = true;
			piece.draw(renderer,texture,SIZE_BLOC);
		}
		
		SDL_Event event;
		while (!quit && SDL_PollEvent(&event) && cont == true)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
				
				// c'est ici quuon prend les événements du clavier.
				// Jai pris sur internet, je sais pas trop comment ca fonctionne sinon
			case SDL_KEYDOWN:
				/* Check the SDLKey values and move change the coords */
				switch( event.key.keysym.sym ){
					//si lutilisateur appuie sur la flèche droite du clavier:
					
				case SDLK_RIGHT:
					piece.right(this->mat);
					if(piece.isLegal(mat)==NO_ERROR) {
						piece.draw(renderer,texture,SIZE_BLOC);
					}
					else
						piece.left(this->mat);
					
					break;
					
				case SDLK_LEFT:
					piece.left(this->mat);
					if(piece.isLegal(mat)==NO_ERROR) {
						piece.draw(renderer,texture,SIZE_BLOC);
					}
					else
						piece.right(this->mat);
					break;
					
				case SDLK_DOWN:
					piece.down(this->mat);
					if(piece.isLegal(mat)==OVER_Y 
							or piece.isLegal(mat)==COLLISION_PIECE) {
						cont = false;
						piece.up(this->mat);
						
						for(int i = 0; i < 4; i++)
							mat[piece.getx(i)][piece.gety(i)]=true;
						this->printMatrice();
					}
					else if(piece.isLegal(mat)==NO_ERROR) {
						piece.draw(renderer,texture, SIZE_BLOC);
					}
					
					break;
					
				case SDLK_UP:
					
					piece.rotateRight();
					if(piece.isLegal(mat)==NO_ERROR) {
						piece.draw(renderer,texture,SIZE_BLOC);
					}
					else
						piece.rotateLeft();
					
					piece.draw(renderer,texture, SIZE_BLOC);

					//piece.up();
					//piece.draw(renderer,texture,SIZE_BLOC);
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
			piece.down(this->mat);
			if (piece.isLegal(mat)==NO_ERROR){
				piece.draw(renderer, texture, SIZE_BLOC);
			}
			else if (piece.isLegal(mat)== COLLISION_PIECE 
					or piece.isLegal(mat)== OVER_Y){
				cont = false;
				piece.up(this->mat);
				for(int i = 0; i < 4; i++)
					mat[piece.getx(i)][piece.gety(i)]=true;
				
				this->printMatrice();
			}
			t=0;
		}
	}

}

void Tetris::printMatrice(){
	std::cout << "L'état de la matrice est : " << std::endl;
	for(int j = 0; j< BLOCSY; j++) {
		for(int i = 0; i< BLOCSX; i++) {
			if (mat[i][j])
				std::cout << "--0--" ;
			else
				std::cout << "--1--" ;
		}
		std::cout << std::endl;
	}
}


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

	SDL_DestroyRenderer(tetris.get_renderer());
	SDL_DestroyWindow(tetris.get_pWindow());
	SDL_Quit();
}
