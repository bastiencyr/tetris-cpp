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


Tetris::Tetris(int w, int h){
	this->w=w;
	this->h=h;

	pWindow = SDL_CreateWindow("Une fenetre SDL" , SDL_WINDOWPOS_CENTERED ,
			SDL_WINDOWPOS_CENTERED , w , h , SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);

	//renderer2 = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED |
	//SDL_RENDERER_PRESENTVSYNC);k
	timer=0;

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	blank = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
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
	
	sizeTetris.w = 250;
	sizeTetris.h = sizeTetris.w*2;
	sizeTetris.x = 50;
	sizeTetris.y = 50;
}

Tetris::~Tetris(){
	SDL_DestroyTexture(blank);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
}

void Tetris::init(){
	//la largeur du tetris doit donc etre un multiple de BLOCSX 
	//la hauteur doit être deux fois plus grande -> respect de lechelle
	int size_bloc = sizeTetris.w/BLOCSX;
	SDL_Point ligne_depart,ligne_arrivee; // Déclaration du point de départ et du point d'arrivée d'une ligne

	//on colorie le fond. La fonction renderDrawColor permet d'initialiser une
	//couleur. Ainsi, toute fonction suivante utilisant une couleur utilisera
	//implicitement cette couleur
	SDL_SetRenderTarget(renderer, blank);

	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	SDL_RenderClear(renderer);

	// À présent, occupons nous des lignes
	// On ne peut pas utiliser la fonction SDL_RenderDrawLines
	// car celle-ci ne permet pas de créer des lignes indépendantes comme nous voulons le faire mais des chemins

	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	// Lignes horizontales
	ligne_depart.x = sizeTetris.x;
	ligne_arrivee.x = sizeTetris.w + sizeTetris.x; //h
	ligne_depart.y = sizeTetris.y ;

	while(ligne_depart.y<sizeTetris.h + sizeTetris.y ) //h
	{
		ligne_depart.y += size_bloc;
		ligne_arrivee.y = ligne_depart.y;
		SDL_RenderDrawLine(renderer,ligne_depart.x, ligne_depart.y,ligne_arrivee.x,ligne_arrivee.y);
	}

	// Lignes verticales
	ligne_depart.x = sizeTetris.x;
	ligne_depart.y = sizeTetris.y ;
	ligne_arrivee.y = sizeTetris.h + sizeTetris.y ; //h

	while(ligne_depart.x<sizeTetris.w + sizeTetris.x ){ //h
		ligne_depart.x += size_bloc;
		ligne_arrivee.x = ligne_depart.x;
		SDL_RenderDrawLine(renderer,ligne_depart.x, ligne_depart.y,ligne_arrivee.x,ligne_arrivee.y);
	}
	
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, blank, &sizeTetris, &sizeTetris);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, &sizeTetris, &sizeTetris);
}


void Tetris::loop()
{
	Uint64 prev, now = SDL_GetPerformanceCounter(); // timers
	double delta_t;  // durée frame en ms


	Piece * PiecList[7];
	PiecList[0] = new LTetri();
	PiecList[1] = new OTetri();
	PiecList[2] = new TTetri();
	PiecList[3] = new ZTetri();
	PiecList[4] = new JTetri();
	PiecList[5] = new ITetri();
	PiecList[6] = new STetri();

	for(int i = 0; i<7; i++)
		PiecList[i]->update();

	int randn=0;

	Piece *piece = new Piece();

	bool quit = false;
	bool cont = true;
	double t=0;
	int score = 0;
	while (!quit)
	{
		if(!cont) {
			std::cout << "Nouvelle pièce en haut" << std::endl;
			randn = rand() % 7;
			std::cout<<randn<<std::endl;
			PiecList[randn]->update();
			piece->affiche_coord(1,1);
			piece = PiecList[randn];
			//if(randn) {
			//	std::cout<<"ltetri"<<std::endl;
			//	ltetri.update();
			//	piece = &ltetri;
			//}
			//else {
			//	std::cout<<"otetri"<<std::endl;
			//
			//	otetri.update();
			//	piece = &otetri;
			//}

			cont = true;
			if(!piece->isLegalPosition(piece, mat).NO_ERROR) {
				piece->draw(renderer,blank,texture);
				quit=true;
			}
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
					if (piece->isLegalRight(mat).NO_ERROR){
						piece->right();
						piece->draw(renderer,blank,texture);
					}
					break;

				case SDLK_LEFT:
					if (piece->isLegalLeft(mat).NO_ERROR){
						piece->left();
						piece->draw(renderer,blank,texture);
					}
					break;

				case SDLK_DOWN:
					cont = piece->onDown(mat, cont, renderer, blank,
							texture);
					break;

				case SDLK_UP:
					if (piece->isLegalRotateRight(mat).NO_ERROR){
						piece->rotateRight();
						piece->draw(renderer,blank,texture);

					}
					else if (piece->isLegalRotateRight(mat).OVER_X){
						Piece temp;
						for (int i=0 ; i<4 ; i++){
							temp.dst[i].x=piece->dst[i].x;
							temp.dst[i].y=piece->dst[i].y;
						}
						int shift = -piece->isLegalRotateRight(mat).OVER_NUMBER_X;
						std::cout << "shift is " << shift << std::endl;
						temp.rotateRight();
						if (temp.isLegalTranslate(shift, 0, mat).NO_ERROR){
							std::cout << "rotation a doite" << std::endl;
							piece->rotateRight();
							piece->translate(shift, 0, false);
							piece->draw(renderer,blank,texture);
						}
						else
							std::cout << "une autre erreur" << std::endl;
					}

					break;
				}

			default: break;
			}

			//this->printMatrice();
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		quit |= (state[SDL_SCANCODE_ESCAPE]!=0);

		prev = now;
		now = SDL_GetPerformanceCounter();
		delta_t = static_cast<double>((now - prev)/
				static_cast<float>(SDL_GetPerformanceFrequency()));

		t+=delta_t;
		if(floor(t)>=1) {
			cont = piece->onDown(mat, cont, renderer, 
					blank,texture);
			//this->printMatrice();
			t=0;
		}

		switch(TetrisLinesUpdate()) {
			case 1:
				score+=100;
				break;
			case 2:
				score+=300;
				break;
			case 3:
				score+=500;
				break;
			case 4:
				score+=800;
				break;
			default:
				break;
			}
		SDL_RenderPresent(renderer);

	}
	std::cout<< "Game Over" << std::endl << "Score : " << score << std::endl;

}

int Tetris::TetrisLinesUpdate() {
	int decalage = 0;
	for(int i = BLOCSY-1; i>=0; i--) {
		int compt = 0;
		for(int j = 0; j<BLOCSX; j++) {
			if(mat[j][i]) compt++;
		}
		if(compt==BLOCSX) //ligne pleine
		{
			decalage++;
			FillEmpty(i,SIZE_BLOC);
		}

		else if(compt < BLOCSX && compt != 0 && decalage!=0)
		{
			CopyLine(i, decalage, SIZE_BLOC);
			FillEmpty(i,SIZE_BLOC);
		}
		//else
		//	break;
	}
	return decalage;
}

void Tetris::FillEmpty(int i,int factor) {
	SDL_Rect line;
	line.x= 0;
	line.y= i*factor;
	line.h= 1*factor;
	line.w= BLOCSX*factor;
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, blank, &line, &line);
	SDL_SetRenderTarget(renderer, NULL);

	for(int j = 0; j<BLOCSX; j++) {
		mat[j][i]=false;
	}
}

void Tetris::CopyLine(int i, int decalage, int factor) {
	SDL_Texture* temp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, this->w, this->h);
	//(je sais pas si ça marche) On copie la texture originelle sur le truc temporaire (y a une vraie raison à ça)
	SDL_SetRenderTarget(renderer, temp);
	SDL_RenderCopy(renderer, blank, NULL, NULL);

	SDL_Rect copyline;
	SDL_Rect copytext;

	copyline.h= factor;
	copyline.w= factor*BLOCSX;
	copyline.y= (i+decalage)*factor;
	copyline.x= 0;

	copytext.h= factor;
	copytext.w= factor;
	copytext.y= i*factor;
	copytext.x= 0;

	for(int j = 0; j < BLOCSX; j++) {
		if(mat[j][i]) {
			copytext.x=j*factor;
			SDL_RenderCopy(renderer, texture, &copytext, &copytext);
			mat[j][i+decalage]=mat[j][i];
			mat[j][i]=0;
		}
	}

	SDL_SetRenderTarget(renderer, texture);
	//SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture,
		//const SDL_Rect* srcrect, const SDL_Rect* dstrect)
		//srcrect rect dans la texture

	copytext.w= factor*BLOCSX;
	copytext.x=0;
	SDL_RenderCopy(renderer, temp, &copytext, &copyline);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_DestroyTexture(temp);

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
