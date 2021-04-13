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
	bool MenuLancement(int h, int w,Mix_Music* music,SDL_Rect sizeTetris);
	void parametresmain(SDL_Renderer* renderer, Tetris &tetris);
	void parametresaudio(SDL_Renderer* renderer, Tetris &tetris);
	void parametresgraph(SDL_Renderer* renderer, Tetris &tetris);
	void fillblankoptions(SDL_Rect * rect, unsigned int options, SDL_Renderer * renderer,  SDL_Texture * graphmenu,SDL_Texture * blankmenu);
	void drawprev(Piece * prev, unsigned int options, SDL_Rect* rect,
				SDL_Renderer * renderer, SDL_Texture * blankmenu, SDL_Texture * graphmenu);

	void DrawCheckboxes(SDL_Renderer * renderer, SDL_Texture * graphmenu,Tetris &tetris);
private:
	Mix_Music* music;

};

#endif
