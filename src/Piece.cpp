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
	    this->src[i].x=floor(BLOCSX/2);
		this->src[i].y=i;
		this->src[i].w=1;
		this->src[i].h=1;

		//Le constructeur par défaut de pièce devra faire apparitre une pièce
		//au hasard. C'est donc dst.x qui doit être choisi au hasard
		this->dst[i].x=floor(BLOCSX/2);
		this->dst[i].y=i;
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
		src_r[i].y=((this->src[i].y)-1)*factor;
		src_r[i].w=this->src[i].w*factor;
		src_r[i].h=this->src[i].h*factor;

		dst_r[i].x=this->dst[i].x*factor;
		dst_r[i].y=((this->dst[i].y)-1)*factor;
		dst_r[i].w=this->dst[i].w*factor;
		dst_r[i].h=this->dst[i].h*factor;

		SDL_RenderCopy(renderer, texture, &src_r[i], &src_r[i]);
	}
	for(int i=0; i<4; i++) {
		SDL_RenderFillRect(renderer, &dst_r[i]);
	}
	SDL_RenderPresent(renderer);

}

bool Piece::translate(int a, int b, bool mat[BLOCSX][BLOCSY]){
	for(int i = 0; i < 4; i++) {
		this->src[i].x=this->dst[i].x;
		this->src[i].y=this->dst[i].y;
		this->src[i].w=this->dst[i].w;
		this->src[i].h=this->dst[i].h;

		this->dst[i].x+=a;
		this->dst[i].y+=b;
	}

	if(!(this->depassement(mat))) {
		std::cout << "mouvement dépassant" << std::endl;
		this->translate(-a, -b, mat);
	}
	if(!(this->isLegal(mat))) {
		std::cout << "mouvement illégal" << std::endl;
		this->translate(-a, -b, mat);
		return false;
	}
	return true;
}

/*
 * Cette fonction déplace une pièce vers le bas sans la dessiner.
 * Il faut appeler la méthode draw pour dessiner la pièce.
 */
bool Piece::down(bool mat[BLOCSX][BLOCSY]){
	//normalement c'est 1 (normalisé). Faudra le faire
	return this->translate(0,1,mat);
}

bool Piece::right(bool mat[BLOCSX][BLOCSY]){
	return this->translate(1,0,mat);
}

bool Piece::left(bool mat[BLOCSX][BLOCSY]){
	return this->translate(-1,0,mat);
}

/*
 * Cette méthode est utile pour revenir en arrière si un déplacement était pas
 * légal. En effet, si l'utilisateur veut se déplacer vers le bas, on change
 * dabord la position de la pièces SANS lafficher; on vérifie la légalité puis on
 * affiche. Si le déplacement n'était pas légal, il faut pouvoir revenir en arrière.
 */
void Piece::up(bool mat[BLOCSX][BLOCSY]){
	this->translate(0,-1,mat);
}

/*
 *Cette fonction effectue la rotation de la pièce.
 * Le centre de rotation est donné par l'entier contenu dans la structure
 * de la pièce.
 */
void Piece::rotate(double alpha){

}

/*
 * Cette méthode sert juste à ce que la pièce ne parte pas sur les côtés
 * La fonction translate l'appelle et replace correctement la pièce si
 * cela arrive
 */
bool Piece::depassement(bool mat[BLOCSX][BLOCSY]){
	for(int i = 0; i< 4; i++) {
		//Verification dépassement horizontal
		if(this->dst[i].x < 0 || this->dst[i].x >= BLOCSX) {
			std::cout << "mouvement illégal (dh)" << std::endl;
			return false;
		}
	}
	return true;
}

/*
 * Cette méthode calcule la légalité d'une pièce sur le plateau.
 * Une position pas légale est une pièce en dehors du plateau ou qui
 * chevauche une autre pièce.
 * Cette fonction doit renvoyer true si le déplacement est légal est faux sinon.
 */
bool Piece::isLegal(bool mat[BLOCSX][BLOCSY]){
		for(int i = 0; i< 4; i++) {
		//Verification dépassement vertical
		if(this->dst[i].y < 0 || this->dst[i].y > BLOCSY) {
			std::cout << "mouvement illégal (dv)" << std::endl;
			return false;
		}
		//Verification occupation de la case
		else if(mat[this->dst[i].x][this->dst[i].y]) {
			std::cout << "mouvement illégal (oc)" << std::endl;
			return false;
		}

	}
	std::cout << "mouvement légal" << std::endl;
	return true;
}


/*
 * Cette méthode affiche les coordonnées d'une pièce
 * Elle a un objectif de débugage
 */
void Piece::affiche_coord(bool source, bool dest){
	if(source) {
		std::cout << "coordonnées de la source ";
		for(int i=0; i< 4; i++) {
			std::cout << "(" <<  this->src[i].x << ", " << this->src[i].y << ")";
			std::cout << "    (taille : (" << this->src[i].w << ",";
			std::cout << this->src[i].h << "))" << std::endl;
		}
	}
	if(dest) {
		std::cout << "coordonnées de la destination";
		for(int i=0; i< 4; i++) {
			std::cout << "(" <<  this->dst[i].x << ", " << this->dst[i].y << ")";
			std::cout << "    (taille : (" << this->dst[i].w << ",";
			std::cout << this->dst[i].h << "))" << std::endl;
		}
	}
}
/*
void Piece::affiche_dst(){
	for(int i=0; i< 4; i++) {
		std::cout << "(" <<  this->dst[i].x << ", " << this->dst[i].y << ")";
		std::cout << "    (taille : (" << this->dst[i].w << ",";
		std::cout << this->dst[i].h << "))" << std::endl;
	}
}*/

int Piece::getx(int i) {
	return this->src[i].x;
}

int Piece::gety(int i) {
	return this->src[i].y;
}

/*############################################################################
########################          LEFT L         #############################
############################################################################*/

LeftL::LeftL(int w) : Piece(w) {
	this->src[0].x=floor(w/2);
	this->src[1].x=floor(w/2)-1;
	this->src[2].x=floor(w/2)-1;
	this->src[3].x=floor(w/2)-1;

	this->src[0].y=0;
	this->src[1].y=0;
	this->src[2].y=1;
	this->src[3].y=2;
}
