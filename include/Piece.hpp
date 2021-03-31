/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#ifndef _FUNC_HPP_
#define _FUNC_HPP_

#include <SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include "Error.hpp"
#define BLOCSX 10
#define BLOCSY 20
#define SIZE_BLOC 35

#define EYES 0
#define MILIEU 0
#define CLASSIC 0
#define ACCESS 0
#define PASTEL 0
#define WHITE_LINED 1

//class Sprite;

class Piece
{
public:
	//une pièce doit donc être deux vecteurs de rectangles. Un vecteur source
	//et un vecteur destination
    SDL_Rect src[4];
	SDL_Rect dst[4];
	SDL_Rect locTetris;
	int color[3];
	int pastel[3];

	//il faudrait aussi quune pièce ait une couleur.
public :
    Piece(SDL_Rect locTetris);
    virtual ~Piece(); // destructor

	virtual void update();
	void adjust(Piece *piece);
	bool verif(Piece * ref);
	void DownGhost(bool mat[BLOCSX][BLOCSY],Piece * ref, bool gen=false);

	int getx(int i);
	int gety(int i);

	int getcol(int i);
	int getpastel(int i);

	bool translate(int a, int b, bool moveSource=true);
    bool down(bool moveSource=true);
	void up(bool moveSource=true);
	bool right(bool moveSource=true);
	bool left(bool moveSource=true);
	void rotateLeft(bool moveSource=true);
	virtual void rotateRight(bool moveSource=true);

	Error isLegalPosition(Piece *temp, bool mat[BLOCSX][BLOCSY]);
	Error isLegalTranslate(int a, int b, bool mat[BLOCSX][BLOCSY]);
	Error isLegalRight(bool mat[BLOCSX][BLOCSY]);
	Error isLegalLeft(bool mat[BLOCSX][BLOCSY]);
	Error isLegalDown(bool mat[BLOCSX][BLOCSY]);
	Error isLegalUp(bool mat[BLOCSX][BLOCSY]);
	Error isLegalRotateLeft(bool mat[BLOCSX][BLOCSY]);
	virtual Error isLegalRotateRight(bool mat[BLOCSX][BLOCSY]);

	bool onDown(bool mat[BLOCSX][BLOCSY], bool cont, SDL_Renderer* renderer,SDL_Texture* blank,
	SDL_Texture* texture);

	void draw(SDL_Renderer* renderer,SDL_Texture*  blank, SDL_Texture* texture, int alpha = 255, bool erase=false);
	void printNextPiece(SDL_Renderer* renderer, SDL_Texture* texture);

	void affiche_coord(bool source, bool dest);

	void cheat(bool mat[BLOCSX][BLOCSY]);

};

class JTetri : public Piece {
	public:
		JTetri(SDL_Rect sizeTetris);
		void update();
};

class LTetri : public Piece {
	public:
		LTetri(SDL_Rect sizeTetris);
		void update();
};

class ITetri : public Piece {
	public:
		ITetri(SDL_Rect sizeTetris);
		void update();
};


class OTetri : public Piece {
	public:
		OTetri(SDL_Rect sizeTetris);
		void update();
		void rotateRight(bool moveSource=true);
		Error isLegalRotateRight(bool mat[BLOCSX][BLOCSY]);

};

class TTetri : public Piece {
	public:
		TTetri(SDL_Rect sizeTetris);
		void update();
};


class ZTetri : public Piece {
	public:
		ZTetri(SDL_Rect sizeTetris);
		void update();
};


class STetri : public Piece {
	public:
		STetri(SDL_Rect sizeTetris);
		void update();
};
#endif
