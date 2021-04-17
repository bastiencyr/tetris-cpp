/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "../include/Error.hpp"
class Piece;
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
	/**
	 * @brief This function is used in multiplayer mode only.
	 * This function add lines to player2 if player2 is true and player1 
	 * either. It draws the line in tetris of the ohter player. 
	 * 
	 * @param nbLineToAdd  is the number of line to add.
	 * 
	 * @param piece is the piece of the other player. This function has to 
	 * redraw it.
	 * @param ghost is the ghost piece f the other player. It is necessary to
	 * reraw it if a line is added.
	 * @param player2. This function add line to IA if it is True and 
	 * to main player either.
	 */
	void addLineToPlayer(int nbLineToAdd, Piece *piece, Piece *ghost, bool player2=false);
	
	/**
	 * @brief This function print the score of player or players.
	 * There is two flags multiplayer and playerIA because the score is not
	 * not print at the same place.   
	 * 
	 * @param multiplayer if true, score is printed on the center of the 
	 * window.
	 * @param playerIA
	 */
	void printScore(bool multiplayer, bool playerIA=false);
	
	/**
	 * @brief This function print "score" on the window. 
	 * @param multiplayer if true, "score" is printed on the center of the 
	 * window. If false, the score is printed on the right.
	 * 
	 * @param playerIA
	 */
	void printScoreText(bool multiplayer, bool playerIA = false);
	
	/**
	 * @brief This function print the level of a player.
	 * 
	 * @param multiplayer if true, the level is not printed. 
	 * Else it is printed on the right of the tetris game.
	 * @param playerIA useless param.
	 */
	void printLevel(bool multiplayer, bool playerIA=false);
	
	/**
	 * @brief This function print "level"
	 * 
	 * @param multiplayer if true, "level" is not printed. 
	 * Else it is printed on the right of the tetris game.
	 * @param playerIA useless param.
	 */
	void printLevelText(bool multiplayer, bool playerIA = false);
	
	/**
	 * This function update the score of the player by comparing his 
	 * current score and his scoreOld
	 * @param ScoreOld
	 */
	void updateLevel(int& scoreOld);
	
	/**
	 * This function print a line to divide the score between player and IA.
	 */
	void printSeparation();
	
	/*
	 * This function restart the game in loop. For example, 
	 * the score is reset.
	 */
	void restart();

	SDL_Rect getLocTetris(){ return locTetris;};
	int getScore(){return score;};
	double getCurrentDifficulty(){return difficulte[difficulte_i];};
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
	SDL_Rect& getLocLevelInt(){ return locLevelInt;};
	
public:
	bool matGame[BLOCSX][BLOCSY];

private:
	SDL_Rect locTetris; //position du tetris central
	SDL_Rect locHelp; //position du help en bas a droite
	SDL_Rect locScoreInt; //position du score
	SDL_Rect locScoreTxt; //position de "score"
	SDL_Rect locLevelTxt; //position de "score"
	SDL_Rect locLevelInt; //position de "score"
	
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
