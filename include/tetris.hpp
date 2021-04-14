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

enum Options {
  EYES = 0x01,
  MILIEU = 0x02,
  WHITE_LINED = 0x04,
  CLASSIC = 0x08,
  ACCESS = 0x10,
  PASTEL = 0x20,
};


class Tetris
{
protected:
    SDL_Renderer* renderer;
    //SDL_Renderer* welcome_screen;
	SDL_Texture *blank;
    SDL_Texture *texture;
	SDL_Texture *menu;

	int volume;
	unsigned int options;

	SDL_Rect sizeTetris;
	SDL_Rect sizeTetris2;
	int w,h;
    double timer;
	bool quitgame; //TODO A SUPPRIMER


public :
	Tetris(int w, int h, SDL_Rect sizeTetris, SDL_Renderer * &renderer, bool multiplayer = false);

	/**************************************************************************/
	//GETTERS
	bool getquit() {return quitgame;};
	int geth() {return h;};
	int getw() {return w;};
	int getopt() {return options;};
	SDL_Renderer* get_renderer() {return renderer;};
	SDL_Texture* get_menu() {return menu;};
	int getvolume() {return volume;};

	ReturnCodeMenu loop(Mix_Music* music, bool multiplayer);

    void init(Mix_Music* music, bool multiplayer = false);
	void ListePieceInit(Piece * Liste[7]);
    void keyboard(const Uint8* keys);

	/*************************************************************************/
	//MENU FUNCTIONS
	bool printGenericMenu(SDL_Texture * text_texture, int xShift,
		int sizeBetweenText, bool retour, int numItem,...);
	ReturnCodeMenu printMenu();
	void UpDownCasesLoopMenu(int retour, int way, int & choiceMenu ,
		int numberChoice, int sizeBetweenText, int xShift,SDL_Rect & cadre, bool vol = false) ;
	void addmenuoptions(SDL_Texture * menu, int xShift, int sizeBetweenText,
			int numberChoice, int indice, int numItem,const char * str1, const char*str2);
	void minimenu(SDL_Texture * menu, SDL_Rect * cadre);
	void DrawSelected();
	ReturnCodeMenu endGameMenu(Mix_Music* music, bool multiplayer, bool win);

	/**************************************************************************/

	void setoption(unsigned int opt) {options ^= opt;};
	void setmodedefault() {options&=~CLASSIC; options&=~ACCESS; options&=~PASTEL;};
	void setmode(unsigned int opt) {options&=~CLASSIC; options&=~ACCESS; options&=~PASTEL; options|=opt;};
	int upvolume() {if(volume<MIX_MAX_VOLUME) volume++; return volume;};
	int downvolume() {if(volume >0) volume --; return volume;};


	void printMatrice();
    ~Tetris(); // destructor


};

#endif
