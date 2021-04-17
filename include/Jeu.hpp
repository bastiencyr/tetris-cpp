/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#ifndef _PIECE_HPP_
#define _PIECE_HPP_

class Jeu
{
public:
	void startTetris(int h, int w, SDL_Rect sizeTetris, bool multiplayer = false);

	/**************************************************************************/
	//The followings functions handle the menus

	/**
	 * @brief This function is the main menu. It initializes the window and
	 * the renderer, as well as the Tetris object.
	 */
	bool MenuLancement(int h, int w,Mix_Music* music,SDL_Rect sizeTetris);
	/**
	 * @brief This function allows access to the audio and graphic parameters.
	 */
	void parametresmain(SDL_Renderer* renderer, Tetris &tetris);
	/**
	 * @brief This function handles the volume modification
	 */
	void parametresaudio(SDL_Renderer* renderer, Tetris &tetris);
	/**
	 * @brief This function handles the graphic modifications. The "mode"
	 * allows to chose the main graphic mode, and the options to be checked
	 * are used to tweak details.
	 */
	void parametresgraph(SDL_Renderer* renderer, Tetris &tetris);

	/**************************************************************************/
	//The followings functions are used by the menu functions

	/**
	 * @brief This function is called by drawprev. It draws the background for
	 * the piece previsualization in the graphical menu.
	 */
	void fillblankoptions(SDL_Rect * rect, unsigned int options, SDL_Renderer * renderer,  SDL_Texture * graphmenu,SDL_Texture * blankmenu);
	/**
	 * @brief This function draw the previsualization
	 */
	void drawprev(Piece * prev, unsigned int options, SDL_Rect* rect,
				SDL_Renderer * renderer, SDL_Texture * blankmenu, SDL_Texture * graphmenu);
	/**
	 * @brief This function draws the checkboxes of the options chosen
	 */
	void DrawCheckboxes(SDL_Renderer * renderer, SDL_Texture * graphmenu,Tetris &tetris);
private:
	Mix_Music* music;

};

#endif
