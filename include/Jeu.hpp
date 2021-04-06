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
	void parametresmain(SDL_Renderer* renderer, Tetris &tetris, TTF_Font * P1, TTF_Font * P2);
	void parametresaudio(SDL_Renderer* renderer, Tetris &tetris, TTF_Font * P1, TTF_Font * P2);
	void parametresgraph(SDL_Renderer* renderer, Tetris &tetris, TTF_Font * P1, TTF_Font * P2);

	void DrawCheckboxes(SDL_Renderer* renderer, Tetris tetris);
private:
	Mix_Music* music;

};

#endif
