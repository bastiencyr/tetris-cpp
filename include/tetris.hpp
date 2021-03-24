/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef _TETRIS_HPP_
#define _TETRIS_HPP_

#include <SDL.h>
#include <string>
#include "Piece.hpp"

class Tetris
{
protected:

    SDL_Window* pWindow;
    SDL_Renderer* renderer;
    //SDL_Renderer* welcome_screen;
	SDL_Texture *blank;
    SDL_Texture *texture;

	//Piece *piece;

	int w;
    int h;
    double timer;

	bool mat[BLOCSX][BLOCSY];

//    SDL_Rect src;
//    SDL_Rect dest;

public :
    void init();
    Tetris(int w, int h);
    void keyboard(const Uint8* keys);
    void loop();
	SDL_Renderer* get_renderer() {return renderer;};
	SDL_Window* get_pWindow() {return pWindow;};
	void printMatrice();

	void TetrisLinesUpdate();
	void FillEmpty(int i, int factor);
	void CopyLine(int i, int decalage, int factor);

    ~Tetris(); // destructor


};

#endif
