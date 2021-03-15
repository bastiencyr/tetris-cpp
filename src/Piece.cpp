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
    this->src.x=41;
	this->src.y=41;
	this->src.w=38;
	this->src.h=38;
	
	this->dst.x=41;
	this->dst.y=0;
	this->dst.w=38;
	this->dst.h=38;
    
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
void Piece::draw(SDL_Renderer* renderer, int factor){
	
	//on efface la pièce source
	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	SDL_RenderFillRect(renderer, &this->src);
	
	//on dessine la pièce cible
	SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255);
	SDL_RenderFillRect(renderer, &this->dst);
	
	SDL_RenderPresent(renderer);
	
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
	//this->translate(0,1);
	this->translate(0,40);
}

void Piece::right(){
	this->translate(40,0);
}


void Piece::left(){
	this->translate(-40,0);
}

/*
 * Cette méthode est utile pour revenir en arrière si un déplacement était pas 
 * légal. En effet, si l'utilisateur veut se déplacer vers le bas, on change 
 * dabord la position de la pièces SANS lafficher; on vérifie la légalité puis on
 * affiche. Si le déplacement n'était pas légal, il faut pouvoir revenir en arrière.
 */
void Piece::up(){
	this->translate(0,-40);
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