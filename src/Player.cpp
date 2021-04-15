/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cassert>
#include <time.h>
#include <SDL_image.h>
#include <SDL.h>
#include "../include//Player.hpp"
#include "../include/Piece.hpp"
#include "../include/Error.hpp"
#include "../include/tetris.hpp"
#include <SDL2/SDL_ttf.h>
#define OPAC 70

#define FREE_SURFACE(surface_t) { SDL_FreeSurface(surface_t); surface_t = nullptr;}
#define FREE_TEXTURE(texture_t) { SDL_DestroyTexture(texture_t); texture_t = nullptr;}
#define FREE_RENDERER_AND_WINDOW(renderer_t, window_t) { \
SDL_DestroyRenderer(renderer_t); SDL_DestroyWindow(window_t); \
renderer_t = nullptr; window_t = nullptr;}

#define BLOCSX 10
#define BLOCSY 20

Player::Player(SDL_Renderer * renderer, SDL_Texture *texture, 
		SDL_Texture *blank, SDL_Rect locTetris, int options){
	this->blank = blank;
	this->texture = texture;
	this->renderer = renderer;
	
	liste[0] = new LTetri(options);
	liste[1] = new OTetri(options);
	liste[2] = new TTetri(options);
	liste[3] = new ZTetri(options);
	liste[4] = new JTetri(options);
	liste[5] = new ITetri(options);
	liste[6] = new STetri(options);
	
	for(int i = 0; i<7; i++)
		liste[i]->update();
	
	//mat[BLOCSX][BLOCSY];
	for(auto &raw : matGame){
		for(auto &el : raw)
			el = false;
	}
	this->w = 3*locTetris.w;
	this->h = locTetris.h;
	this->locTetris.w = locTetris.w;
	this->locTetris.h = locTetris.w*2;
	this->locTetris.x = locTetris.x;
	this->locTetris.y = locTetris.y;
	score = 0 ;
	difficulte_i=0;
	
}

ReturnCodeMenu Player::nouvPiece(SDL_Renderer* renderer, SDL_Texture* texture, Piece * & oldp, Piece *& newp) {
	ReturnCodeMenu gameState = ReturnCodeMenu::INIT ;
	oldp = newp;
	oldp->update();

	int randn = rand() % 7;
	newp = liste[randn];
	newp->update();
	newp->printNextPiece2(renderer, blank, texture);

	if(!oldp->isLegalPosition(oldp, matGame).NO_ERROR) {
		gameState = ReturnCodeMenu::GAME_OVER ;
		std::cout<< "Game Over" << std::endl << "Score : " << score << std::endl;
	}
	else
		oldp->draw(renderer,blank,texture);

	return gameState;
}

void Player::restart(SDL_Renderer* renderer, SDL_Texture* texture){
	score = 0 ;
	for(int i = 0; i < BLOCSX; i++) {
		for(int j = 0; j < BLOCSY; j++) {
			matGame[i][j] = false;
		}
	}
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, blank, &locTetris, &locTetris);
}

void Player::updateLevel(int& ScoreOld){
	if (score-ScoreOld>500) {
		ScoreOld=score;
		if(difficulte_i!=17) {
			difficulte_i++;
			std::cout << "Niveau supérieur !" << std::endl;
			std::cout << "		Niveau :" << difficulte_i << std::endl;
		}
	}
}




int Player::tetrisLinesUpdate(SDL_Renderer* renderer, SDL_Texture* texture) {
	
	int decalage = 0;
	for(int i = BLOCSY-1; i>=0; i--) {
		
		int compt = 0;
		for(int j = 0; j<BLOCSX; j++)
			if (matGame[j][i])
				compt++;
		
		if(compt==BLOCSX){ //ligne pleine
			decalage++;
			fillEmpty(renderer, texture, i);
		}
		
		
		else if(compt < BLOCSX && compt != 0 && decalage!=0)
		{
			copyLine(renderer, texture, i, decalage);
			fillEmpty(renderer, texture, i);
		}
		//else
		//	break;
	}
	if(decalage>=1){
		std::cout << decalage << std::endl;
		printMatrice();
	}
	switch(decalage) {
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
	if(decalage!=0) {
		std::cout << "Bravo !" << std::endl;
		std::cout << "Score : " << score << std::endl;
	}
	return decalage;
}


void Player::fillEmpty(SDL_Renderer* renderer, SDL_Texture* texture, int i) {
	int factor = locTetris.w/BLOCSX;
	SDL_Rect line;
	line.x= 0 + locTetris.x;


	line.y= i*factor+ locTetris.y;
	line.h= 1*factor;
	line.w= BLOCSX*factor;
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, blank, &line, &line);
	SDL_SetRenderTarget(renderer, NULL);
	
	for (auto &row : matGame)
		row[i]=false;
	
}

void Player::copyLine(SDL_Renderer* renderer, SDL_Texture* texture, int i, int decalage) {
	int factor = locTetris.w/BLOCSX;
	SDL_Texture* temp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, this->w, this->h);
	//(je sais pas si ça marche) On copie la texture originelle sur le truc temporaire (y a une vraie raison à ça)
	SDL_SetRenderTarget(renderer, temp);
	SDL_RenderCopy(renderer, blank, NULL, NULL);
	
	SDL_Rect copyline;
	SDL_Rect copytext;
	
	copyline.h= factor;
	copyline.w= factor*BLOCSX;
	copyline.y= (i+decalage)*factor+ locTetris.y;
	copyline.x= 0 + locTetris.x;
	
	copytext.h= factor;
	copytext.w= factor;
	copytext.y= i*factor+ locTetris.y;
	copytext.x= 0 + locTetris.x;
	
	for(int j = 0; j < BLOCSX; j++) {
		if(matGame[j][i]) {
			copytext.x=j*factor + locTetris.x;
			SDL_RenderCopy(renderer, texture, &copytext, &copytext);
			matGame[j][i+decalage]=matGame[j][i];
			matGame[j][i]=0;
		}
	}
	
	SDL_SetRenderTarget(renderer, texture);
	
	copytext.w= factor*BLOCSX;
	copytext.x=0 + locTetris.x;
	
	SDL_RenderCopy(renderer, temp, &copytext, &copyline);
	
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	
	FREE_TEXTURE(temp);
	
}

void Player::printMatrice(){
	std::cout << "L'état de la matrice est : " << std::endl;
	for(int j = 0; j< BLOCSY; j++) {
		for(int i = 0; i< BLOCSX; i++) {
			if (matGame[i][j])
				std::cout << "--0--" ;
			else
				std::cout << "--1--" ;
		}
		std::cout << std::endl;
	}
}

void Player::addLineToPlayer(SDL_Renderer* renderer, SDL_Texture* texture, int nbLineToAdd, Piece *piece, Piece *ghost, bool player2){
	//player2 = true -> ajouter une ligne à l'IA
	for(int i=0 ; i< nbLineToAdd ;i++){
		SDL_SetRenderDrawColor(renderer,100,100,100,255);
		srand(time(0));
		int randn= rand() % 10;
		
		int factor = locTetris.w/BLOCSX;
		//ajouter une ligne à player1
		for (int i = 0; i< BLOCSY ; i++)
			copyLine(renderer, texture, i, -1);
		
		for(int j=0 ; j< BLOCSX ; j++)
			matGame[j][BLOCSY-1] = true;
		matGame[randn][BLOCSY-1] = false;
	
		
		for(int j = 0; j< BLOCSX ; j++){
			SDL_SetRenderTarget(renderer, texture);
			
			SDL_Rect line = {
				locTetris.x + j*factor,
				(BLOCSY-1)*factor+ locTetris.y,
				factor,
				factor,
			};
			
			SDL_RenderCopy(renderer, blank, &line, &line);
			
			if(j!=randn){
				line.x= locTetris.x + j*factor + 5;
				line.y= (BLOCSY-1)*factor+ locTetris.y + 5;
				line.h= factor - 10;
				line.w= factor - 10;
				if (player2) line.x= locTetris.x + j*factor + 5;
				
				SDL_RenderFillRect(renderer, &line);
				SDL_SetRenderTarget(renderer, NULL);
				SDL_RenderCopy(renderer, texture, &line, &line);
			}
		}
		piece->up();
		
		//A laisser ?
		piece->mvDstToSrc(*piece);
		
		if(!player2){
			ghost->up();
			ghost->DownGhost(matGame,piece);
			ghost->verif(piece);
			
			ghost->draw(renderer,blank,texture,OPAC);
			piece->draw(renderer,blank,texture);
		}
		SDL_RenderPresent(renderer);
	}
}

void Player::printScore(SDL_Renderer* renderer, SDL_Texture* texture, int xScore, int yScore){

	TTF_Font * police = TTF_OpenFont("src/RetroGaming.ttf", 65);
	if(!police){
		std::cout << TTF_GetError()<< std::endl;
	}

	//affchage de "score"
	SDL_Color textColor = {255, 255, 255};
	SDL_Surface * text_surface = TTF_RenderText_Blended(police, "Score", textColor);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	int scoreW = text_surface->w/2, scoreH = text_surface->h/2;
	SDL_Rect dstrect = {0,0, scoreW, scoreH };

	dstrect.x = xScore;
	dstrect.y = yScore;

	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, &dstrect, &dstrect);

	std::string scoreStr = std::to_string ( score );
	char * scoreStrArr = &scoreStr[0];

	FREE_SURFACE(text_surface);
	FREE_TEXTURE(text_texture);

	//print score
	text_surface = TTF_RenderText_Solid(police, scoreStrArr, textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect dstrect2 = {0,0, text_surface->w/2, text_surface->h/2 };
	dstrect2.x = xScore + scoreW/2 - text_surface->w/4 ;
	dstrect2.y = yScore + 60;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderFillRect(renderer, &dstrect2);

	//on affiche une ligne de séparation
	//if (player2){
	//	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	//	SDL_RenderDrawLine(renderer,3*sizeTetris.w/2, 60,
	//			3*sizeTetris.w/2,250);
	//}

	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect2);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, &dstrect2, &dstrect2);

	TTF_CloseFont(police);
	FREE_SURFACE(text_surface);
	FREE_TEXTURE(text_texture);
	police = nullptr;

}