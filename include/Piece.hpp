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

/*
#define EYES 0
#define MILIEU 1
#define CLASSIC 0
#define ACCESS 0
#define PASTEL 0
#define WHITE_LINED 0*/

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
	unsigned int opt;

	//il faudrait aussi quune pièce ait une couleur.
public :
	Piece(unsigned int options = 0x0);
	static void initStaticMembers(SDL_Rect sizeTetris);
    virtual ~Piece(); // destructor

	/**************************************************************************/
	//The getters
	int getx(int i);
	int gety(int i);

	int getcol(int i);
	int getpastel(int i);

	/**************************************************************************/

	virtual void update();
	void adjust(Piece *piece);
	void verif(Piece * ref);
	void DownGhost(bool mat[BLOCSX][BLOCSY],Piece * ref, bool gen=false);

	void setopt(unsigned int options) {this->opt = options;};

	/**
	 * @brief This function copied dst array attribute of this in src attribute
	 * of pieceDst
	 * @param pieceDst The piece in which the src is moved.
	 */
	void mvDstToSrc(Piece &pieceDst);

	//The folowwings functions move an entire piece
	/**
	 * @brief This function translate a piece. A piece composes of two arrays
	 * (src and dst) of four SDL_Rect. When a piece is translated, the dst is
	 * move on src and dst is translated.
	 * @param a, the translation along x axis
	 * @param b, the translation along y axis
	 * @param moveDstToSource if true, the dst is copied into src.
	 * @return always true.
	 */
	bool translate(int a, int b, bool moveDstToSource=true);

	/**
	 * @brief This function call translate(0,1,moveSource). Cf translate for
	 * more detail.
	 * @param moveSource
	 * @return
	 */
    bool down(bool moveSource=true);

	/**
	 * @brief This function call translate(0,-1,moveSource). Cf translate for
	 * more detail.
	 * @param moveSource
	 */
	void up(bool moveSource=true);

	/**
	 * @brief This function call translate(1,0,moveSource). Cf translate for
	 * more detail.
	 * @param moveSource
	 */
	bool right(bool moveSource=true);

	/**
	 * @brief This function call translate(0,1,moveSource). Cf translate for
	 * more detail.
	 * @param moveSource
	 */
	bool left(bool moveSource=true);

	/**
	 * This function copied dst into src if moveSource is true and changes
	 * dst so that this is rotated to the right.
	 * @param moveSource
	 */
	void rotateLeft(bool moveSource=true);

	/**
	 * This function copied dst into src if moveSource is true and changes
	 * dst so that this is rotated to the right.
	 * @param moveSource
	 */

	virtual void rotateRight(bool moveSource=true);

	/**************************************************************************/
	//The followings functions ensure the legality of pieces movements

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

	/**
	 * @brief This function erases the old position of the piece, and
	 * draws the new one, following to the graphic options used.
	 * @param mat the matrix of the Tetris
	 * @param renderer, blank, texture used to draw
	 * @param alpha opacity
	 * @param erase if true, draw only erases the old position and doesn't
	 * draw the new one
	 * @param shift a x-shift in a % of a case
	 * This function works on the "texture" texture and copies the result
	 * to the renderer. The texture "blank" is used for the erasing part.
	 * This function is used wherever a piece needs being drawn, and thus
	 * uses some control mechanism, for example so that the "ghost" piece
	 * doesn't risk erasing the piece that's falling.
	 */
	void draw(SDL_Renderer* renderer,SDL_Texture*  blank, SDL_Texture* texture,
	int alpha = 255, bool erase=false, float shift = 0.);

	/**
	 * @brief This function handles pieces being drawn in a square
	 * @param square The square to be drawn into
	 * @param a, b Integers to be used to translate the piece into the square
	 * and positioning it in a neat way
	 */
	virtual void printinsquare(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture, SDL_Rect* square, int a, int b, bool init=false,float xShift=0.5);
	void printNextPiece2(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture);
	void printreserve(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture, bool multiplayer, bool init=false);

	void cheat(bool mat[BLOCSX][BLOCSY]);

	/**
	 * @brief This function move dst array piece on down. This functions permits
	 * to implement ghost feature.
	 * src array is not
	 * changed.
	 * @param mat the matrice that represents tetris game.
	 */
	void holdPiece(bool mat[BLOCSX][BLOCSY]);

	/**
	 * @brief This function return the number of full line in a tetris game.
	 * A Tetris is represented  as a matrice of BLOCSx*BLOCSY. A full line
	 * is a line where all element are true.
	 * @param mat the matrice that represents element.
	 * @return the number of full line in mat.
	 */
	int nbFullLine(bool mat[BLOCSX][BLOCSY]);

	void affiche_coord(bool source, bool dest);

};

class JTetri : public Piece {
	public:
		JTetri(unsigned int options );
		void update() override;
};

class LTetri : public Piece {
	public:
		LTetri(unsigned int options );
		void update();
		void placeprev(int x, int y);
};

class ITetri : public Piece {
	public:
		ITetri(unsigned int options );
		void update() override;
		//void printNextPiece2(SDL_Renderer* renderer,
		//SDL_Texture*  blank,SDL_Texture* texture,float xShift=0.5) override;
		void printinsquare(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture, SDL_Rect* square, int a, int b, bool init, float xShift=0.5) override;
};


class OTetri : public Piece {
	public:
		OTetri(unsigned int options );
		void update();
		void rotateRight(bool moveSource=true);
		Error isLegalRotateRight(bool mat[BLOCSX][BLOCSY]);
		//void printNextPiece2(SDL_Renderer* renderer,
		//SDL_Texture*  blank,SDL_Texture* texture, float xShift = 0.9);
		void printinsquare(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture, SDL_Rect* square, int a, int b, bool init, float xShift=0.9);


};

class TTetri : public Piece {
	public:
		TTetri(unsigned int options );
		void update();
};


class ZTetri : public Piece {
	public:
		ZTetri(unsigned int options );
		void update();
};


class STetri : public Piece {
	public:
		STetri(unsigned int options );
		void update();
};
#endif
