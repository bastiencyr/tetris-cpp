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
    ~Piece(); // destructor
    bool down(bool mat[BLOCSX][BLOCSY]);
	void up(bool mat[BLOCSX][BLOCSY]);
	bool right(bool mat[BLOCSX][BLOCSY]);
	bool left(bool mat[BLOCSX][BLOCSY]);
	void draw(SDL_Renderer* renderer);
	void draw(SDL_Renderer* renderer,SDL_Texture*  texture, int factor);
	bool translate(int a, int b, bool mat[BLOCSX][BLOCSY]);
	void rotate(double alpha);
	error isLegal(bool mat[BLOCSX][BLOCSY]);
	//bool depassement(bool mat[BLOCSX][BLOCSY]);
	void affiche_coord(bool source, bool dest);
	int getx(int i);
	int gety(int i);
};

class JTetri : public Piece {
	JTetri(int w);
};

class LTetri : public Piece {
	LTetri(int w);
};

class ITetri : public Piece {
	ITetri(int w);
};

class OTetri : public Piece {
	OTetri(int w);
};

class TTetri : public Piece {
	TTetri(int w);
};

class ZTetri : public Piece {
	ZTetri(int w);
};

class STetri : public Piece {
	STetri(int w);
};
#endif
