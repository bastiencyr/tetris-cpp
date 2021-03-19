/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#ifndef _FUNC_HPP_
#define _FUNC_HPP_

#include <SDL.h>
#include <string>
#include "Error.hpp"
#define SIZE_BLOC 35
#define BLOCSX 10
#define BLOCSY 20

//class Sprite;

class Piece
{
protected:
	//une pièce doit donc être deux vecteurs de rectangles. Un vecteur source
	//et un vecteur destination
    SDL_Rect src[4];
	SDL_Rect dst[4];

	//il faudrait aussi quune pièce ait une couleur.
public :
    Piece(int w);
    virtual ~Piece(); // destructor
	
	virtual void update();
	
	int getx(int i);
	int gety(int i);
	
	bool translate(int a, int b);
    bool down();
	void up();
	bool right();
	bool left();
	void rotateLeft();
	void rotateRight();
	
	bool onDown(bool mat[BLOCSX][BLOCSY], bool cont, SDL_Renderer* renderer,
	SDL_Texture* texture);
	
	error isLegal(bool mat[BLOCSX][BLOCSY]);
	void draw(SDL_Renderer* renderer,SDL_Texture*  texture, int factor);

	void affiche_coord(bool source, bool dest);
	
	
};

//class JTetri : public Piece {
//public:
//	JTetri(int w);
//};

class LTetri : public Piece {
public:
	LTetri(int w);
	void update();
};

//class ITetri : public Piece {
//	ITetri(int w);
//};
//
//class OTetri : public Piece {
//	OTetri(int w);
//};
//
//class TTetri : public Piece {
//	TTetri(int w);
//};
//
//class ZTetri : public Piece {
//	ZTetri(int w);
//};
//
//class STetri : public Piece {
//	STetri(int w);
//};
#endif
