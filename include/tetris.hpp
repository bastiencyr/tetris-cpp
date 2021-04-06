/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef _TETRIS_HPP_
#define _TETRIS_HPP_

#include <SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include "Piece.hpp"

#define SIZE_BLOC 35
#define BLOCSX 10
#define BLOCSY 20

class Tetris
{
protected:

    SDL_Window* pWindow;
    SDL_Renderer* renderer;
    //SDL_Renderer* welcome_screen;
	SDL_Texture *blank;
    SDL_Texture *texture;
	SDL_Texture *menu;
	int volume;
	unsigned int options;

	//Piece *piece;

	SDL_Rect sizeTetris;
	SDL_Rect sizeTetris2;
	int w;
    int h;
    double timer;
	bool quit;
	bool quitgame;
	int score;


	bool mat[BLOCSX][BLOCSY];
	bool matIA[BLOCSX][BLOCSY];

//    SDL_Rect src;
//    SDL_Rect dest;

public :
	Tetris(int w, int h, SDL_Rect sizeTetris, SDL_Renderer *renderer, bool multiplayer = false);

    void init(Mix_Music* music, bool multiplayer = false);
	void ListePieceInit(Piece * Liste[7]);
    void keyboard(const Uint8* keys);

    bool loop(Mix_Music* music, bool multiplayer);
	void NouvPiece(Piece *& oldp, Piece *& newp, Piece * Liste[7]);

	void printMatrice();

	int TetrisLinesUpdate(int* score, bool player2 = false);
	void FillEmpty(int i, int factor, bool player2 = false);
	void CopyLine(int i, int decalage, int factor, bool player2 = false);
	void addLineToPlayer(int nbLineToAdd, Piece *piece, Piece *ghost, bool player2 = false);

	bool printMenu();

	bool printGenericMenu(SDL_Texture * text_texture, int xShift,
	int sizeBetweenText, bool retour, int numItem,...);
	void UpDownCasesLoopMenu(int retour, int way, int & choiceMenu ,
		int numberChoice, int sizeBetweenText, int xShift,SDL_Rect & cadre, bool vol = false) ;
	void addmenuoptions(SDL_Texture * menu, int xShift, int sizeBetweenText,
			int numberChoice, int indice, int numItem,const char * str1, const char*str2);
	void minimenu(SDL_Texture * menu, SDL_Rect * cadre);

	bool getquit() {return quitgame;};
	int geth() {return h;};
	int getw() {return w;};
	int getopt() {return options;};
	SDL_Renderer* get_renderer() {return renderer;};
	SDL_Window* get_pWindow() {return pWindow;};
	SDL_Texture* get_menu() {return menu;};
	int getvolume() {return volume;};
	void setoption(unsigned int opt) {options ^= opt;};
	int upvolume() {if(volume<MIX_MAX_VOLUME) volume++; return volume;};
	int downvolume() {if(volume >0) volume --; return volume;};

    ~Tetris(); // destructor


};

enum Options {
  EYES = 0x01,
  MILIEU = 0x02,
  WHITE_LINED = 0x04,
  CLASSIC = 0x08,
  ACCESS = 0x10,
  PASTEL = 0x20,
};

#endif
