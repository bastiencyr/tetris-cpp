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
	ReturnCodeMenu nouvPiece(Piece * & oldp, Piece *& newp);

	 ~Player(); // destructor

	 /**************************************************************************/
 	//The followings functions handle the lines disappearing when completed

	/**
	 * This function detects the filled lines and calls the following two
	 * functions when needed
	 * @return int number of lines completed
	 */
	int tetrisLinesUpdate();
	/**
	 * The function erases a line
	 * @param int i the index of the line to be erased
	 */
	void fillEmpty(int i);
	/**
	 * This function moves a line
	 * @param int i the line to be moved
	 * @param int decalage number of lines down to be translated to
	 */
	void copyLine(int i, int decalage);

	void printMatrice();
	void addLineToPlayer(int nbLineToAdd, Piece *piece, Piece *ghost, bool player2=false);
	void printScore(bool multiplayer, bool playerIA=false);
	void printScoreText(bool multiplayer, bool playerIA = false);
	void updateLevel(int& ScoreOld);
	void printSeparation();
	void restart();



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

	SDL_Rect& getLocScoreInt(){ return locScoreInt;};

public:
	bool matGame[BLOCSX][BLOCSY];

private:
	SDL_Rect locTetris; //position du tetris central
	SDL_Rect locHelp; //position du help en bas a droite
	SDL_Rect locScoreInt; //position du score
	SDL_Rect locScoreTxt; //position de "score"
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
