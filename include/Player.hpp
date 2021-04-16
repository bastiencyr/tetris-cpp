/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "../include/Error.hpp"
class Piece;
class MatGame;
#define BLOCSX 10
#define BLOCSY 20

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

class Player
{
public:
	Player(SDL_Renderer * renderer, SDL_Texture *texture, 
		SDL_Texture *blank, SDL_Rect locTetris, int options);
	ReturnCodeMenu nouvPiece(SDL_Renderer* renderer, SDL_Texture* texture, Piece * & oldp, Piece *& newp);

	int tetrisLinesUpdate(SDL_Renderer* renderer, SDL_Texture* texture);
	void fillEmpty(SDL_Renderer* renderer, SDL_Texture* texture, int i);
	void copyLine(SDL_Renderer* renderer, SDL_Texture* texture, int i, int decalage);
	void printMatrice();
	void addLineToPlayer(SDL_Renderer* renderer, SDL_Texture* texture, int nbLineToAdd, Piece *piece, Piece *ghost, bool player2=false);
	void printScore(SDL_Renderer* renderer, SDL_Texture* texture, int xScore, int yScore);
	void updateLevel(int& ScoreOld);
	
	void restart(SDL_Renderer* renderer, SDL_Texture* texture);


	
	SDL_Rect getLocTetris(){ return locTetris;};
	int getScore(){return score;};

	double getCurrentDifficulty(){return difficulte[difficulte_i];};
	//MatGame* getMat(){ return matGame;};
	
	Piece *getRandomPiece(){
		srand(time(0));
		int randn= rand() % 7;
		return liste[randn];
	}
	Piece *getPiece(int i){
		return liste[i];
	}
	
	Piece ** getListe(){ return liste;}
	
public:
	bool matGame[BLOCSX][BLOCSY];

private:
	SDL_Rect locTetris;
	SDL_Rect locHelp;
	int w,h;
	int score;
	int difficulte_i;
	double difficulte[18]={1.,0.9,0.82,0.72,0.61,0.52,0.45,0.4,0.37,0.35,0.34,0.33,0.32,0.31,0.3,0.29,0.28,0.27}; //difficulté
	Piece *liste[7];
	SDL_Texture *blank;
	SDL_Renderer* renderer;
    SDL_Texture *texture;
};

#endif

