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
    void down();
	void up();
	void right();
	void left();
	void draw(SDL_Renderer* renderer);
	void draw(SDL_Renderer* renderer, int factor);
	void translate(int a, int b);
	void rotate(double alpha);
	bool isLegal();
    
};

#endif