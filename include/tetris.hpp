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
    SDL_Renderer* renderer2;
    SDL_Surface* winSurf;
    SDL_Texture *texture;

	int w;
    int h;
    double timer;

	bool mat[BLOCSX][BLOCSY];

//    SDL_Rect src;
//    SDL_Rect dest;

	Piece piece;

public :
    void init();
    Tetris(int w, int h);
    void keyboard(const Uint8* keys);
    void loop();
	SDL_Renderer* get_renderer() {return renderer;};
	SDL_Window* get_pWindow() {return pWindow;};

    ~Tetris(); // destructor


};

#endif
