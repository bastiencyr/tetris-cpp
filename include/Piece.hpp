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
	//une pièce doit donc être deux vecteurs de rectangles. Un vecteur source
	//et un vecteur destination
    SDL_Rect src;
	SDL_Rect dst;
	//il faudrait aussi quune pièce ait une couleur. 
public :
    Piece();
    ~Piece(); // destructor
    void down();
	void up();
	void right();
	void left();
	void draw(SDL_Renderer* renderer);
	void draw(SDL_Renderer* renderer,SDL_Texture*  texture, int factor);
	void translate(int a, int b);
	void rotate(double alpha);
	bool isLegal();
    
};

#endif