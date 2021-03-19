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

#include "../include/Error.hpp"

Piece::Piece(int id) {
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
		src_r[i].y=((this->src[i].y))*factor;
		src_r[i].w=this->src[i].w*factor;
		src_r[i].h=this->src[i].h*factor;

		dst_r[i].x=this->dst[i].x*factor;
		dst_r[i].y=((this->dst[i].y))*factor;
		dst_r[i].w=this->dst[i].w*factor;
		dst_r[i].h=this->dst[i].h*factor;

		SDL_RenderCopy(renderer, texture, &src_r[i], &src_r[i]);
	}
	for(int i=0; i<4; i++) {
		SDL_RenderFillRect(renderer, &dst_r[i]);
	}
	SDL_RenderPresent(renderer);

}

bool Piece::translate(int a, int b){
	for(int i = 0; i < 4; i++) {
		this->src[i].x=this->dst[i].x;
		this->src[i].y=this->dst[i].y;
		this->dst[i].x+=a;
		this->dst[i].y+=b;
	}
	return true;
}

/*
 * Cette fonction déplace une pièce vers le bas sans la dessiner.
 * Il faut appeler la méthode draw pour dessiner la pièce.
 */
bool Piece::down(){
	//normalement c'est 1 (normalisé). Faudra le faire
	return this->translate(0, 1);
}

bool Piece::right(){
	return this->translate(1, 0);
}

bool Piece::left(){
	return this->translate(-1, 0);
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
void Piece::rotateLeft(){

	for(int i = 0; i<4; i++) {
		this->src[i].x=this->dst[i].x;
		this->src[i].y=this->dst[i].y;
	}

	//ATTENTION, il faut bien séparer les deux boucles !!
	for(int i = 0; i<4; i++) {
		this->dst[i].x = this->src[i].y - this->src[1].y + this->src[1].x;
		this->dst[i].y= this->src[1].x - this->src[i].x + this->src[1].y;
	}
}

/*
 *Cette fonction effectue la rotation de la pièce.
 * Le centre de rotation est donné par l'entier contenu dans la structure
 * de la pièce.
 */
void Piece::rotateRight(){

	for(int i = 0; i<4; i++) {
		this->src[i].x=this->dst[i].x;
		this->src[i].y=this->dst[i].y;
	}

	//ATTENTION, il faut bien séparer les deux boucles !!
	for(int i = 0; i<4; i++) {
		this->dst[i].x = this->src[1].y - this->src[i].y + this->src[1].x;
		this->dst[i].y= this->src[i].x - this->src[1].x + this->src[1].y;
	}
}

bool Piece::onDown(bool mat[BLOCSX][BLOCSY], bool cont, SDL_Renderer* renderer,
	SDL_Texture* texture){

	this->down();
	if(this->isLegal(mat)==OVER_Y
			or this->isLegal(mat)==COLLISION_PIECE) {
		cont = false;

		this->up();
		for(int i = 0; i < 4; i++)
			mat[this->getx(i)][this->gety(i)]=true;
	}
	else if(this->isLegal(mat)==NO_ERROR) {
		this->draw(renderer,texture, SIZE_BLOC);
	}
	return cont;
}


/*
 * Cette méthode calcule la légalité d'une pièce sur le plateau.
 * Une position pas légale est une pièce en dehors du plateau ou qui
 * chevauche une autre pièce.
 * Cette fonction doit renvoyer true si le déplacement est légal est faux sinon.
 */
error Piece::isLegal(bool mat[BLOCSX][BLOCSY]){

	error e = ERROR;

	for(int i = 0; i< 4; i++) {
		//Verification dépassement vertical
		if(this->dst[i].y < 0 || this->dst[i].y == BLOCSY) {
			std::cout << "mouvement illégal (dv)" << std::endl;
			e = OVER_Y;
			return e;
			//return false;
		}
		//Verification occupation de la case
		else if(mat[this->dst[i].x][this->dst[i].y]) {
			std::cout << "mouvement illégal (oc)" << std::endl;
			e = COLLISION_PIECE;
			return e;
			//return false;
		}

		//vérification dépassement horizontal
		else if(this->dst[i].x < 0 || this->dst[i].x == BLOCSX) {
			std::cout << "mouvement illégal (dh)" << std::endl;
			e = OVER_X;
			return e;
		}

	}
	std::cout << "mouvement légal" << std::endl;
	e = NO_ERROR;
	return e;
	//return true;
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
	return this->dst[i].x;
}

int Piece::gety(int i) {
	return this->dst[i].y;
}
 void Piece::update(){
	 puts("update de la classe mère!!!");
 }

/*############################################################################
########################          LEFT L         #############################
############################################################################*/

//JTetri::JTetri(int w) : Piece(w){
//	this->src[0].x=floor(w/2);
//	this->src[1].x=floor(w/2)-1;
//	this->src[2].x=floor(w/2)-1;
//	this->src[3].x=floor(w/2)-1;
//
//	this->dst[0].y=0;
//	this->dst[1].y=0;
//	this->dst[2].y=1;
//	this->dst[3].y=2;
//}

LTetri::LTetri(int w) : Piece(w){

	for(int i = 0; i<4; i++) {
	    this->src[i].x=floor(BLOCSX/2);
		this->src[i].y=i;


		//Le constructeur par défaut de pièce devra faire apparitre une pièce
		//au hasard. C'est donc dst.x qui doit être choisi au hasard
		this->dst[i].x=floor(BLOCSX/2);
		this->dst[i].y=i;

	}

	this->dst[3].x=floor(BLOCSX/2)+1;
	this->dst[3].y= 2;
}

void LTetri::update(){

	for(int i = 0; i<4; i++) {
	    this->src[i].x=floor(BLOCSX/2);
		this->src[i].y=i;

		this->dst[i].x=floor(BLOCSX/2);
		this->dst[i].y=i;

	}

	this->dst[3].x=floor(BLOCSX/2)+1;
	this->dst[3].y= 2;

}

OTetri::OTetri(int w) : Piece(w) {
}

void OTetri::update() {
	this->src[0].x=floor(BLOCSX/2);
	this->src[1].x=floor(BLOCSX/2);
	this->src[2].x=floor(BLOCSX/2)+1;
	this->src[3].x=floor(BLOCSX/2)+1;

	this->src[0].y=0;
	this->src[1].y=1;
	this->src[2].y=0;
	this->src[3].y=1;

	this->dst[0].x=floor(BLOCSX/2);
	this->dst[1].x=floor(BLOCSX/2);
	this->dst[2].x=floor(BLOCSX/2)+1;
	this->dst[3].x=floor(BLOCSX/2)+1;

	this->dst[0].y=0;
	this->dst[1].y=1;
	this->dst[2].y=0;
	this->dst[3].y=1;
}
//ITetri::ITetri(int w) : Piece(w) {
//
//}

//void ITetri::update() {
//	his->src[0].x=floor(w/2);
//	this->src[1].x=floor(w/2);
//	this->src[2].x=floor(w/2);
//	this->src[3].x=floor(w/2);
//
//	this->src[0].y=0;
//	this->src[1].y=1;
//	this->src[2].y=2;
//	this->src[3].y=3;
//}

//ITetri::ITetri(int w) : Piece(w){
//	this->src[0].x=floor(w/2);
//	this->src[1].x=floor(w/2);
//	this->src[2].x=floor(w/2);
//	this->src[3].x=floor(w/2);
//
//	this->src[0].y=0;
//	this->src[1].y=1;
//	this->src[2].y=2;
//	this->src[3].y=3;
//}
//
//OTetri::OTetri(int w) : Piece(w) {
//	this->src[0].x=floor(w/2);
//	this->src[1].x=floor(w/2);
//	this->src[2].x=floor(w/2)+1;
//	this->src[3].x=floor(w/2)+1;
//
//	this->src[0].y=0;
//	this->src[1].y=1;
//	this->src[2].y=0;
//	this->src[3].y=1;
//}
//
//TTetri::TTetri(int w) : Piece(w) {
//	this->src[0].x=floor(w/2);
//	this->src[1].x=floor(w/2)+1;
//	this->src[2].x=floor(w/2)+2;
//	this->src[3].x=floor(w/2)+1;
//
//	this->src[0].y=0;
//	this->src[1].y=0;
//	this->src[2].y=0;
//	this->src[3].y=1;
//}
//
//
//ZTetri::ZTetri(int w) : Piece(w) {
//	this->src[0].x=floor(w/2);
//	this->src[1].x=floor(w/2)+1;
//	this->src[2].x=floor(w/2)+1;
//	this->src[3].x=floor(w/2)+2;
//
//	this->src[0].y=0;
//	this->src[1].y=0;
//	this->src[2].y=1;
//	this->src[3].y=1;
//}
//
//
//STetri::STetri(int w) : Piece(w) {
//	this->src[0].x=floor(w/2)+1;
//	this->src[1].x=floor(w/2)+1;
//	this->src[2].x=floor(w/2);
//	this->src[3].x=floor(w/2);
//
//	this->src[0].y=1;
//	this->src[1].y=1;
//	this->src[2].y=0;
//	this->src[3].y=0;
//}
