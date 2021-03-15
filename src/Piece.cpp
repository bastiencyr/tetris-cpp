/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <SDL.h>
#include <string>
#include <iostream>
#include <cassert>

#include "../include/Piece.hpp"

Piece::Piece(int w) {
	//au début la position de la source na pas dimportance, on peut linitialiser à
	// des paramètres quelconque
	for(int i = 0; i<4; i++) {
	    this->src[i].x=floor(w/2);
		this->src[i].y=i;
		this->src[i].w=1;
		this->src[i].h=1;

		//Le constructeur par défaut de pièce devra faire apparitre une pièce
		//au hasard. C'est donc dst.x qui doit être choisi au hasard
		this->dst[i].x=floor(w/2);
		this->dst[i].y=0;
		this->dst[i].w=1;
		this->dst[i].h=1;
	}

}

Piece::~Piece() {
}

/*
 Cette fonction est chargée de dessiner une pièce sur le renderer.
 Elle ne vérifie pas si elle a le droit de dessiner.
 */
void Piece::draw(SDL_Renderer* renderer){

	for(int i = 0; i < 4; i++) {
	//on efface la pièce source
		SDL_SetRenderDrawColor(renderer,63,63,63,255);
		SDL_RenderFillRect(renderer, &this->src[i]);

		//on dessine la pièce cible
		SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255);
		SDL_RenderFillRect(renderer, &this->dst[i]);

		SDL_RenderPresent(renderer);
	}

}

/*
 * Fait la même chose que a fonction en haut mais avec un facteur déchelle
 * Y a surement moyen de mettre des arguments optionnel mais jai pas trouvé
 *
 Cette fonction est chargée de dessiner une pièce sur le renderer.
 Elle ne vérifie pas si elle a le droit de dessiner.
 */
void Piece::draw(SDL_Renderer* renderer,SDL_Texture*  texture, int factor){


	SDL_Rect src_r[4];
	SDL_Rect dst_r[4];
	SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255); /* On dessine en violet */
	for(int i = 0; i < 4; i++) {
		src_r[i].x=this->src[i].x*factor;
		src_r[i].y=this->src[i].y*factor;
		src_r[i].w=this->src[i].w*factor;
		src_r[i].h=this->src[i].h*factor;

		dst_r[i].x=this->dst[i].x*factor;
		dst_r[i].y=this->dst[i].y*factor;
		dst_r[i].w=this->dst[i].w*factor;
		dst_r[i].h=this->dst[i].h*factor;

		SDL_RenderCopy(renderer, texture, &src_r[i], &src_r[i]);
		SDL_RenderFillRect(renderer, &dst_r[i]);
		SDL_RenderPresent(renderer);
	}

}

void Piece::translate(int a, int b){
	for(int i = 0; i < 4; i++) {
		this->src[i].x=this->dst[i].x;
		this->src[i].y=this->dst[i].y;
		this->src[i].w=this->dst[i].w;
		this->src[i].h=this->dst[i].h;

		this->dst[i].x+=a;
		this->dst[i].y+=b;
	}
}

/*
 * Cette fonction déplace une pièce vers le bas sans la dessiner.
 * Il faut appeler la méthode draw pour dessiner la pièce.
 */
void Piece::down(){
	//normalement c'est 1 (normalisé). Faudra le faire
	this->translate(0,1);
}

void Piece::right(){
	this->translate(1,0);
}

void Piece::left(){
	this->translate(-1,0);
}

/*
 * Cette méthode est utile pour revenir en arrière si un déplacement était pas
 * légal. En effet, si l'utilisateur veut se déplacer vers le bas, on change
 * dabord la position de la pièces SANS lafficher; on vérifie la légalité puis on
 * affiche. Si le déplacement n'était pas légal, il faut pouvoir revenir en arrière.
 */
void Piece::up(){
	this->translate(0,-1);
}

/*
 *Cette fonction effectue la rotation de la pièce.
 * Le centre de rotation est donné par l'entier contenu dans la structure
 * de la pièce.
 */
void Piece::rotate(double alpha){

}

/*
 * Cette méthode calcule la légalité d'une pièce sur le plateau.
 * Une position pas légale est une pièce en dehors du plateau ou qui
 * chevauche une autre pièce.
 * Cette fonction doit renvoyer true si le déplacement est légal est faux sinon.
 */
bool Piece::isLegal(){
	return true;
}



/*############################################################################
########################          LEFT L         #############################
############################################################################*/

Left_L::Left_L(int w) : Piece(w) {
	this->src[0].x=floor(w/2);
	this->src[1].x=floor(w/2)-1;
	this->src[2].x=floor(w/2)-1;
	this->src[3].x=floor(w/2)-1;

	this->src[0].y=0;
	this->src[1].y=0;
	this->src[2].y=1;
	this->src[3].y=2;
}
