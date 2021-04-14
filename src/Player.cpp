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

#include <SDL.h>
#include "../include//Player.hpp"
#include "../include/Piece.hpp"
#include "../include/Error.hpp"
#include "../include/tetris.hpp"
#include <SDL2/SDL_ttf.h>

#define FREE_SURFACE(surface_t) { SDL_FreeSurface(surface_t); surface_t = nullptr;}
#define FREE_TEXTURE(texture_t) { SDL_DestroyTexture(texture_t); texture_t = nullptr;}
#define FREE_RENDERER_AND_WINDOW(renderer_t, window_t) { \
SDL_DestroyRenderer(renderer_t); SDL_DestroyWindow(window_t); \
renderer_t = nullptr; window_t = nullptr;}

#define BLOCSX 10
#define BLOCSY 20

Player::Player(SDL_Texture *blank, SDL_Rect locTetris, int options){
	this->blank = blank;
	
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
	
	this->locTetris.w = locTetris.w;
	this->locTetris.h = locTetris.w*2;
	this->locTetris.x = locTetris.x;
	this->locTetris.y = locTetris.y;
	score = 0 ;
	sc=0;
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

void Player::updateLevel(int& ScoreOld){
	if (score-ScoreOld>500) {
		ScoreOld=score;
		if(sc!=17) {
			sc++;
			std::cout << "Niveau supérieur !" << std::endl;
			std::cout << "		Niveau :" << sc << std::endl;
		}
	}
}


int Player::tetrisLinesUpdate(SDL_Renderer* renderer, SDL_Texture* texture, int *score) {
	
	int decalage = 0;
	for(int i = BLOCSY-1; i>=0; i--) {
		
		int compt = 0;
		for(int j = 0; j<BLOCSX; j++)
			matGame[j][i];
		
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
	switch(decalage) {
	case 1:
			*score+=100;
			break;
		case 2:
			*score+=300;
			break;
		case 3:
			*score+=500;
			break;
		case 4:
			*score+=800;
			break;
		default:
			break;
		}
	if(decalage!=0) {
		std::cout << "Bravo !" << std::endl;
		std::cout << "Score : " << *score << std::endl;
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