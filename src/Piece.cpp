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

Piece::Piece() {
	//au début la position de la source na pas dimportance, on peut linitialiser à 
	// des paramètres quelconque
    this->src.x=0;
	this->src.y=0;
	this->src.w=1;
	this->src.h=1;
	
	//Le constructeur par défaut de pièce devra faire apparitre une pièce 
	//au hasard. C'est donc dst.x qui doit être choisi au hasard
	this->dst.x=1;
	this->dst.y=0;
	this->dst.w=1;
	this->dst.h=1;
    
}

Piece::~Piece() { 
}

/*
 Cette fonction est chargée de dessiner une pièce sur le renderer. 
 Elle ne vérifie pas si elle a le droit de dessiner. 
 */
void Piece::draw(SDL_Renderer* renderer){
	
	//on efface la pièce source
	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	SDL_RenderFillRect(renderer, &this->src);
	
	//on dessine la pièce cible
	SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255);
	SDL_RenderFillRect(renderer, &this->dst);
	
	SDL_RenderPresent(renderer);
	
}

/*
 * Fait la même chose que a fonction en haut mais avec un facteur déchelle
 * Y a surement moyen de mettre des arguments optionnel mais jai pas trouvé
 * 
 Cette fonction est chargée de dessiner une pièce sur le renderer. 
 Elle ne vérifie pas si elle a le droit de dessiner. 
 */
void Piece::draw(SDL_Renderer* renderer,SDL_Texture*  texture, int factor){
	
	
	SDL_Rect src_r;
	SDL_Rect dst_r;
	
	src_r.x=this->src.x*factor;
	src_r.y=this->src.y*factor;
	src_r.w=this->src.w*factor;
	src_r.h=this->src.h*factor;
	
	dst_r.x=this->dst.x*factor;
	dst_r.y=this->dst.y*factor;
	dst_r.w=this->dst.w*factor;
	dst_r.h=this->dst.h*factor;
	
	SDL_RenderCopy(renderer, texture, &src_r, &src_r);
	SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255); /* On dessine en violet */
	SDL_RenderFillRect(renderer, &dst_r);
	SDL_RenderPresent(renderer);
	
	//on efface la pièce source
	//SDL_SetRenderDrawColor(renderer,63,63,63,255);
	//SDL_RenderFillRect(renderer, &src_r);
	
	//on dessine la pièce cible
	//SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255);
	//SDL_RenderFillRect(renderer, &dst_r);
	
	//renderer est la cible
	//SDL_SetRenderTarget(renderer, NULL);
	
	//SDL_RenderCopy(renderer, texture, NULL, &dst_r);
	//SDL_RenderPresent(renderer);
	
	
}

void Piece::translate(int a, int b){
	
	this->src.x=this->dst.x;
	this->src.y=this->dst.y; 
	this->src.w=this->dst.w;
	this->src.h=this->dst.h;
	
	this->dst.x+=a;
	this->dst.y+=b;
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