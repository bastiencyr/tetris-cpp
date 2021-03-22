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

Piece::Piece() {
	this->color[0]=0;
	this->color[1]=255;
	this->color[2]=255;
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
	SDL_SetRenderDrawColor(renderer, this->color[0], this->color[1], this->color[2], 255); /* On dessine en violet */
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

bool Piece::translate(int a, int b, bool moveSource){
	if (moveSource){
		for(int i = 0; i < 4; i++) {
			this->src[i].x=this->dst[i].x;
			this->src[i].y=this->dst[i].y;
		}
		
	}
	for(int i = 0; i < 4; i++) {
		this->dst[i].x+=a;
		this->dst[i].y+=b;
	}
	return true;
}

/*
 * Cette fonction déplace une pièce vers le bas sans la dessiner.
 * Il faut appeler la méthode draw pour dessiner la pièce.
 */
bool Piece::down(bool moveSource){
	//normalement c'est 1 (normalisé). Faudra le faire
	return this->translate(0, 1, moveSource);
}

bool Piece::right(bool moveSource){
	return this->translate(1, 0, moveSource);
}

bool Piece::left(bool moveSource){
	return this->translate(-1, 0, moveSource);
}

/*
 * Cette méthode est utile pour revenir en arrière si un déplacement était pas
 * légal. En effet, si l'utilisateur veut se déplacer vers le bas, on change
 * dabord la position de la pièces SANS lafficher; on vérifie la légalité puis on
 * affiche. Si le déplacement n'était pas légal, il faut pouvoir revenir en arrière.
 */
void Piece::up(bool moveSource){
	this->translate(0, -1, moveSource);
}


/*
 *Cette fonction effectue la rotation de la pièce.
 * Le centre de rotation est donné par l'entier contenu dans la structure
 * de la pièce.
 */
void Piece::rotateLeft(bool moveSource){
	
	Piece temp;
	for(int i = 0; i<4; i++) {
		temp.src[i].x=this->dst[i].x;
		temp.src[i].y=this->dst[i].y;
	}
	
	if(moveSource){
		for(int i = 0; i<4; i++) {
			this->src[i].x=this->dst[i].x;
			this->src[i].y=this->dst[i].y;
		}
	}
	
	//ATTENTION, il faut bien séparer les deux boucles !!
	for(int i = 0; i<4; i++) {
		this->dst[i].x = temp.src[i].y - temp.src[1].y + temp.src[1].x;
		this->dst[i].y= temp.src[1].x - temp.src[i].x + temp.src[1].y;
	}
}

/*
 *Cette fonction effectue la rotation de la pièce.
 * Le centre de rotation est donné par l'entier contenu dans la structure
 * de la pièce.
 */
void Piece::rotateRight(bool moveSource){
	
	Piece temp;
	for(int i = 0; i<4; i++) {
		temp.src[i].x=this->dst[i].x;
		temp.src[i].y=this->dst[i].y;
	}
	
	if(moveSource){
		for(int i = 0; i<4; i++) {
			this->src[i].x=this->dst[i].x;
			this->src[i].y=this->dst[i].y;
		}
	}

	//ATTENTION, il faut bien séparer les deux boucles !!
	for(int i = 0; i<4; i++) {
		this->dst[i].x = temp.src[1].y - temp.src[i].y + temp.src[1].x;
		this->dst[i].y= temp.src[i].x - temp.src[1].x + temp.src[1].y;
	}
}

bool Piece::onDown(bool mat[BLOCSX][BLOCSY], bool cont, SDL_Renderer* renderer,
		SDL_Texture* texture){
	
	if(this->isLegalDown(mat).OVER_Y
			or this->isLegalDown(mat).COLLISION_PIECE) {
		cont = false;

		for(int i = 0; i < 4; i++)
			mat[this->getx(i)][this->gety(i)]=true;
	}
	else if(this->isLegalDown(mat).NO_ERROR) {
		this->down();
		this->draw(renderer,texture, SIZE_BLOC);
	}
	return cont;
}

Error Piece::isLegalTranslate(int a, int b, bool mat[BLOCSX][BLOCSY]){
	Piece temp;
	for (int i=0; i< 4; i++){
		temp.dst[i].x=this->dst[i].x + a;
		temp.dst[i].y=this->dst[i].y + b;
	}
	return isLegalPosition(&temp, mat);
}

Error Piece::isLegalPosition(Piece *temp, bool mat[BLOCSX][BLOCSY]){
	
	Error e;
	
	e.ERROR = false;
	e.COLLISION_PIECE = false;
	e.OVER_X = false;
	e.OVER_Y = false;
	e.NO_ERROR = true;
	e.OVER_RIGHT_X = 0;
	e.OVER_LEFT_X = 0;
	e.OVER_UP_y = 0;
	e.OVER_DOWN_y = 0;
	e.OVER_NUMBER_X = 0;
	
	for(int i = 0; i< 4; i++) {
		if(temp->dst[i].x < 0 and temp->dst[i].x < e.OVER_NUMBER_X)
			e.OVER_NUMBER_X = temp->dst[i].x;
		
		else if (temp->dst[i].x >= BLOCSX
				and temp->dst[i].x - BLOCSX +1 > e.OVER_NUMBER_X)
			e.OVER_NUMBER_X = temp->dst[i].x - BLOCSX +1;
	}
	
	for(int i = 0; i< 4; i++) {
		//Verification dépassement vertical
		if(temp->dst[i].y < 0 || temp->dst[i].y == BLOCSY) {
			std::cout << "mouvement illégal (dv)" << std::endl;
			e.OVER_Y=true;
			e.NO_ERROR=false;
		}
		//Verification occupation de la case
		if(mat[temp->dst[i].x][temp->dst[i].y]) {
			std::cout << "mouvement illégal (oc)" << std::endl;
			e.COLLISION_PIECE=true;
			e.NO_ERROR=false;
		}
		
		if(temp->dst[i].x < 0 || temp->dst[i].x >= BLOCSX) {
			std::cout << "mouvement illégal (dh)" << std::endl;
			e.OVER_X=true;
			e.NO_ERROR=false;
		}
	}
	
	return e;
	
}

Error Piece::isLegalRight(bool mat[BLOCSX][BLOCSY]){
	return isLegalTranslate(1, 0, mat);
}

Error Piece::isLegalLeft(bool mat[BLOCSX][BLOCSY]){
	return isLegalTranslate(-1, 0, mat);
}

Error Piece::isLegalDown(bool mat[BLOCSX][BLOCSY]){
	return isLegalTranslate(0, 1, mat);
}

Error Piece::isLegalUp(bool mat[BLOCSX][BLOCSY]){
	return isLegalTranslate(0, -1, mat);
}

/*
 *Cette fonction effectue la rotation de la pièce.
 * Le centre de rotation est donné par l'entier contenu dans la structure
 * de la pièce.
 */
Error Piece::isLegalRotateLeft(bool mat[BLOCSX][BLOCSY]){
	
	Piece temp;

	for(int i = 0; i<4; i++) {
		temp.dst[i].x = this->dst[i].y - this->dst[1].y + this->dst[1].x;
		temp.dst[i].y= this->dst[1].x - this->dst[i].x + this->dst[1].y;
	}
	return isLegalPosition(&temp, mat);
}

/*
 *Cette fonction effectue la rotation de la pièce.
 * Le centre de rotation est donné par l'entier contenu dans la structure
 * de la pièce.
 */
Error Piece::isLegalRotateRight(bool mat[BLOCSX][BLOCSY]){
	
	Piece temp;

	for(int i = 0; i<4; i++) {
		temp.dst[i].x = this->dst[1].y - this->dst[i].y + this->dst[1].x;
		temp.dst[i].y= this->dst[i].x - this->dst[1].x + this->dst[1].y;
	}
	return isLegalPosition(&temp, mat);
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

int Piece::getcol(int i) {
	return this->color[i];
}
 void Piece::update(){
	 puts("update de la classe mère!!!");
 }

/*############################################################################
########################          LEFT L         #############################
############################################################################*/

JTetri::JTetri() : Piece(){
	this->color[0]=0;
	this->color[1]=0;
	this->color[2]=255;
}

void JTetri::update() {
	this->src[0].x=floor(BLOCSX/2);
	this->src[1].x=floor(BLOCSX/2)-1;
	this->src[2].x=floor(BLOCSX/2)-1;
	this->src[3].x=floor(BLOCSX/2)-1;

	this->src[0].y=0;
	this->src[1].y=0;
	this->src[2].y=1;
	this->src[3].y=2;

	this->dst[0].x=floor(BLOCSX/2);
	this->dst[1].x=floor(BLOCSX/2)-1;
	this->dst[2].x=floor(BLOCSX/2)-1;
	this->dst[3].x=floor(BLOCSX/2)-1;

	this->dst[0].y=0;
	this->dst[1].y=0;
	this->dst[2].y=1;
	this->dst[3].y=2;
}

LTetri::LTetri() : Piece(){
	this->color[0]=255;
	this->color[1]=140;
	this->color[2]=0;
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

OTetri::OTetri() : Piece() {
	this->color[0]=255;
	this->color[1]=255;
	this->color[2]=0;
}

void OTetri::update() {
	puts("création carré");
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

Error OTetri::isLegalRotateRight(bool mat[BLOCSX][BLOCSY]){
	puts("ooooooook");
	Error e;
	e.NO_ERROR = true;
	return e;
}

void OTetri::rotateRight(bool moveSource){
}

ITetri::ITetri() : Piece() {
	this->color[0]=0;
	this->color[1]=255;
	this->color[2]=255;
}

void ITetri::update() {
	this->src[0].x=floor(BLOCSX/2);
	this->src[1].x=floor(BLOCSX/2);
	this->src[2].x=floor(BLOCSX/2);
	this->src[3].x=floor(BLOCSX/2);

	this->src[0].y=0;
	this->src[1].y=1;
	this->src[2].y=2;
	this->src[3].y=3;

	this->dst[0].x=floor(BLOCSX/2);
	this->dst[1].x=floor(BLOCSX/2);
	this->dst[2].x=floor(BLOCSX/2);
	this->dst[3].x=floor(BLOCSX/2);

	this->dst[0].y=0;
	this->dst[1].y=1;
	this->dst[2].y=2;
	this->dst[3].y=3;
}


TTetri::TTetri() : Piece() {
	this->color[0]=160;
	this->color[1]=0;
	this->color[2]=160;
}

void TTetri::update() {
	this->src[0].x=floor(BLOCSX/2);
	this->src[1].x=floor(BLOCSX/2)+1;
	this->src[2].x=floor(BLOCSX/2)+2;
	this->src[3].x=floor(BLOCSX/2)+1;

	this->src[0].y=0;
	this->src[1].y=0;
	this->src[2].y=0;
	this->src[3].y=1;

	this->dst[0].x=floor(BLOCSX/2);
	this->dst[1].x=floor(BLOCSX/2)+1;
	this->dst[2].x=floor(BLOCSX/2)+2;
	this->dst[3].x=floor(BLOCSX/2)+1;

	this->dst[0].y=0;
	this->dst[1].y=0;
	this->dst[2].y=0;
	this->dst[3].y=1;
}

ZTetri::ZTetri() : Piece() {
	this->color[0]=255;
	this->color[1]=0;
	this->color[2]=0;
}

void ZTetri::update() {
	this->src[0].x=floor(BLOCSX/2);
	this->src[1].x=floor(BLOCSX/2)+1;
	this->src[2].x=floor(BLOCSX/2)+1;
	this->src[3].x=floor(BLOCSX/2)+2;

	this->src[0].y=0;
	this->src[1].y=0;
	this->src[2].y=1;
	this->src[3].y=1;

	this->dst[0].x=floor(BLOCSX/2);
	this->dst[1].x=floor(BLOCSX/2)+1;
	this->dst[2].x=floor(BLOCSX/2)+1;
	this->dst[3].x=floor(BLOCSX/2)+2;

	this->dst[0].y=0;
	this->dst[1].y=0;
	this->dst[2].y=1;
	this->dst[3].y=1;
}


STetri::STetri() : Piece() {
	this->color[0]=0;
	this->color[1]=255;
	this->color[2]=0;
}


void STetri::update() {
	this->src[0].x=floor(BLOCSX/2);
	this->src[1].x=floor(BLOCSX/2)+1;
	this->src[2].x=floor(BLOCSX/2)+1;
	this->src[3].x=floor(BLOCSX/2)+2;

	this->src[0].y=1;
	this->src[1].y=1;
	this->src[2].y=0;
	this->src[3].y=0;

	this->dst[0].x=floor(BLOCSX/2);
	this->dst[1].x=floor(BLOCSX/2)+1;
	this->dst[2].x=floor(BLOCSX/2)+1;
	this->dst[3].x=floor(BLOCSX/2)+2;

	this->dst[0].y=1;
	this->dst[1].y=1;
	this->dst[2].y=0;
	this->dst[3].y=0;
}
