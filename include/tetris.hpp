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
public:
	
    SDL_Window* pWindow;
    SDL_Renderer* renderer;
    SDL_Renderer* renderer2;
    SDL_Surface* winSurf;
    SDL_Texture *texture;
	
    int w;
    int h;
    double timer;
//    SDL_Rect src;
//    SDL_Rect dest;
	
	Piece piece;

public :
    void init();
    Tetris(int w, int h);
    void keyboard(const Uint8* keys);
    void loop();

    ~Tetris(); // destructor


};

#endif
