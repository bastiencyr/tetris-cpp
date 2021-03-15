/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#ifndef _FUNC_HPP_
#define _FUNC_HPP_

#include <SDL.h>
#include <string>

//class Sprite;

class Piece
{
public:
    SDL_Rect src;
	SDL_Rect dst;
    //SPrite 

public :
    Piece();
    ~Piece(); // destructor
    void down(double &timer, SDL_Renderer* renderer);
	void right(double &timer, SDL_Renderer* renderer);
	void left(double &timer, SDL_Renderer* renderer);
    
};

#endif