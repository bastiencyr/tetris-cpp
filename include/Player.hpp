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
	Player(SDL_Rect locTetris, int options);
	ReturnCodeMenu nouvPiece(SDL_Renderer* renderer, SDL_Texture* texture, Piece * & oldp, Piece *& newp);
	void updateLevel(int& ScoreOld);
	int tetrisLinesUpdate(SDL_Renderer* renderer, SDL_Texture* texture, int *score);
	void fillEmpty(SDL_Renderer* renderer, SDL_Texture* texture, int i);
	void copyLine(SDL_Renderer* renderer, SDL_Texture* texture, int i, int decalage);
	void printMatrice();


private:
	SDL_Rect locTetris;
	int w,h;
	int score;
	int sc;
	bool matGame[BLOCSX][BLOCSY];
	Piece *liste[7];
	SDL_Texture *blank;
};

#endif
