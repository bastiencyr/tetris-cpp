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
#define MILIEU 1
#define CLASSIC 0
#define ACCESS 0
#define PASTEL 0
#define WHITE_LINED 0

//class Sprite;

class Piece
{
public:
	//une pièce doit donc être deux vecteurs de rectangles. Un vecteur source
	//et un vecteur destination
    SDL_Rect src[4];
	SDL_Rect dst[4];
	static SDL_Rect locTetris;
	int color[3];
	int pastel[3];

	//il faudrait aussi quune pièce ait une couleur.
public :
	Piece();
	static void initStaticMembers(SDL_Rect sizeTetris);

    virtual ~Piece(); // destructor

	virtual void update();
	void adjust(Piece *piece);
	void verif(Piece * ref);
	void DownGhost(bool mat[BLOCSX][BLOCSY],Piece * ref, bool gen=false);

	void mvDstToSrc(Piece &pieceDst);
	
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

	/**
	 * @brief This function check the position of the piece temp in the matrice 
	 * mat. It doesnt modify this.
	 * @param temp is the piece to check legality
	 * @param mat is the matrice of tetris. mat[i][j] is true if a piece 
	 * is present, false either.
	 * @return an instance of class Error. Error has the following attributes : 
	 * COLLISION_PIECE is true if a piece is already present in the same location 
	 * of temp.
	 * OVER_X is true if a rectangle from piece exceeds the dimension of mat
	 * along x axis.
	 * OVER_Y is true if a rectangle from piece the dimension of mat
	 * along y axis.
	 * OVER_NUMBER_X is an integer. it measures how much the piece exceeds
	 * the x dimension.
	 * NO_ERROR if temp is in a legal place. 
	 * 
	 */
	Error isLegalPosition(Piece *temp, bool mat[BLOCSX][BLOCSY]);
	
	/**
	 * @brief This function check if a piece can be translated. It 
	 *	creates a copy of 'this', translate it and callls isLegalPostion.   
	 * @param a : translation of a along x axis
	 * @param b : translation of b along y axis
	 * @param mat : the matrice of the tetris game. 
	 * @return an instance of class Error (cf isLegalPostition for 
	 * more explanations)
	 */
	Error isLegalTranslate(int a, int b, bool mat[BLOCSX][BLOCSY]);
	
	/**
	 * @brief This function calls isLegalTranslate (1,0,mat). 
	 * Cf isLegalTranslate.
	 * @param mat
	 * @return isLegalTranslate (1,0,mat)
	 */
	Error isLegalRight(bool mat[BLOCSX][BLOCSY]);
	
	/**
	 * @brief This function calls isLegalTranslate (-1,0,mat). 
	 * Cf isLegalTranslate.
	 * @param mat
	 * @return isLegalTranslate (-1,0,mat)
	 */
	Error isLegalLeft(bool mat[BLOCSX][BLOCSY]);
	
	/**
	 * @brief This function calls isLegalTranslate (0,1,mat). 
	 * Cf isLegalTranslate.
	 * @param mat
	 * @return isLegalTranslate (0,1,mat)
	 */
	Error isLegalDown(bool mat[BLOCSX][BLOCSY]);
	
	/**
	 * @brief This function calls isLegalTranslate (0,-1,mat). 
	 * Cf isLegalTranslate.
	 * @param mat
	 * @return isLegalTranslate (0,-1,mat)
	 */
	Error isLegalUp(bool mat[BLOCSX][BLOCSY]);
	
	/**
	 * @brief This function makes a copy of 'this', rotates is and calls 
	 * isLegalPostion on the copy. So 'this' is not modify.
	 * @param mat
	 * @return an instance of class Error. Cf isLegalPosition for more 
	 * explanations.
	 */
	Error isLegalRotateLeft(bool mat[BLOCSX][BLOCSY]);
	
	/**
	 * @brief This function makes a copy of 'this', rotates is and calls 
	 * isLegalPostion on the copy. So 'this' is not modify.
	 * @param mat
	 * @return an instance of class Error. Cf isLegalPosition for more 
	 * explanations.
	 */
	virtual Error isLegalRotateRight(bool mat[BLOCSX][BLOCSY]);

	bool onDown(bool mat[BLOCSX][BLOCSY], bool cont, SDL_Renderer* renderer,SDL_Texture* blank,
	SDL_Texture* texture);

	void draw(SDL_Renderer* renderer,SDL_Texture*  blank, SDL_Texture* texture, int alpha = 255, bool erase=false);
	void printNextPiece(SDL_Renderer* renderer, SDL_Texture* texture);
	void printNextPiece2(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture);


	void affiche_coord(bool source, bool dest);

	void cheat(bool mat[BLOCSX][BLOCSY]);
	void holdPiece(bool mat[BLOCSX][BLOCSY]);
	int nbFullLine(bool mat[BLOCSX][BLOCSY]);

};

class JTetri : public Piece {
	public:
		JTetri();
		void update() override;
};

class LTetri : public Piece {
	public:
		LTetri();
		void update();
};

class ITetri : public Piece {
	public:
		ITetri();
		void update() override;
};


class OTetri : public Piece {
	public:
		OTetri();
		void update();
		void rotateRight(bool moveSource=true);
		Error isLegalRotateRight(bool mat[BLOCSX][BLOCSY]);

};

class TTetri : public Piece {
	public:
		TTetri();
		void update();
};


class ZTetri : public Piece {
	public:
		ZTetri();
		void update();
};


class STetri : public Piece {
	public:
		STetri();
		void update();
};
#endif
