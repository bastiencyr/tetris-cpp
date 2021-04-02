/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef _TETRIS_HPP_
#define _TETRIS_HPP_

#include <SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include "Piece.hpp"

#define SIZE_BLOC 35
#define BLOCSX 10
#define BLOCSY 20

class Tetris
{
protected:

    SDL_Window* pWindow;
    SDL_Renderer* renderer;
    //SDL_Renderer* welcome_screen;
	SDL_Texture *blank;
    SDL_Texture *texture;
	SDL_Texture *menu;

	//Piece *piece;

	SDL_Rect sizeTetris;
	SDL_Rect sizeTetris2;
	int w;
    int h;
    double timer;
	bool quit;
	bool quitgame;
	int score;


	bool mat[BLOCSX][BLOCSY];
	bool matIA[BLOCSX][BLOCSY];

//    SDL_Rect src;
//    SDL_Rect dest;

public :
	Tetris(int w, int h, SDL_Rect sizeTetris, SDL_Renderer *renderer, bool multiplayer = false);

    void init(Mix_Music* music, bool multiplayer = false);
	void ListePieceInit(Piece * Liste[7]);
    void keyboard(const Uint8* keys);

    bool loop(Mix_Music* music);
	void NouvPiece(Piece *& oldp, Piece *& newp, Piece * Liste[7]);

	void printMatrice();

	int TetrisLinesUpdate(int* score);
	void FillEmpty(int i, int factor);
	void CopyLine(int i, int decalage, int factor);
	bool printMenu();

	bool getquit() {return quitgame;};
	SDL_Renderer* get_renderer() {return renderer;};
	SDL_Window* get_pWindow() {return pWindow;};
	SDL_Texture* get_menu() {return menu;};

    ~Tetris(); // destructor


};

#endif
