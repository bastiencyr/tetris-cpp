/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <cassert>

#include "../include/Piece.hpp"
#include "../include/Error.hpp"
#include "../include/tetris.hpp"

SDL_Rect Piece::locTetris;

Piece::Piece(unsigned int options) {
	this->opt = options;
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

void Piece::initStaticMembers(SDL_Rect sizeTetris){
	locTetris.w = sizeTetris.w;
	locTetris.h = locTetris.w*2;
	locTetris.x = sizeTetris.x;
	locTetris.y = sizeTetris.y;
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

void Piece::draw(SDL_Renderer* renderer,SDL_Texture*  blank,SDL_Texture*  texture,
		int alpha, bool erase, float shift){
	//pour que la transparence soit prise en compte
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if(alpha!=255 && this->opt & ACCESS) alpha=130;
	int factor = locTetris.w/BLOCSX;

	SDL_Rect src_r[4];
	SDL_Rect dst_r[4];
	SDL_Rect blanc[4];
	SDL_Rect eyes[4][2];

	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, this->color[0], this->color[1], this->color[2], alpha); /* On dessine en violet */
	for(int i = 0; i < 4; i++) {
		src_r[i].x=(this->src[i].x+shift)*factor + locTetris.x +1 ;
		src_r[i].y=((this->src[i].y))*factor + locTetris.y+1;
		src_r[i].w=this->src[i].w*factor;
		src_r[i].h=this->src[i].h*factor;

		dst_r[i].x=(this->dst[i].x + shift)*factor + locTetris.x+1;
		dst_r[i].y=((this->dst[i].y))*factor + locTetris.y+1;
		dst_r[i].w=this->dst[i].w*factor-1;
		dst_r[i].h=this->dst[i].h*factor-1;

		if(this->opt & (ACCESS | CLASSIC | MILIEU)) {
			blanc[i].x=(this->dst[i].x+shift)*factor + locTetris.x + 0.2*factor+1;
			blanc[i].y=((this->dst[i].y))*factor + locTetris.y + 0.2*factor+1;
			blanc[i].w=this->dst[i].w*factor - 0.4*factor-1;
			blanc[i].h=this->dst[i].h*factor - 0.4*factor-1;
		}
		else {
			blanc[i].x=(this->dst[i].x+shift)*factor + locTetris.x + 0.1*factor+1;
			blanc[i].y=((this->dst[i].y))*factor + locTetris.y + 0.1*factor+1;
			blanc[i].w=this->dst[i].w*factor - 0.3*factor-1;;
			blanc[i].h=this->dst[i].h*factor - 0.3*factor;-1;
		}

		if(this->opt & EYES) {
			eyes[i][0].x=(this->dst[i].x+shift)*factor + locTetris.x + 0.25*factor+1;
			eyes[i][0].y=((this->dst[i].y))*factor + locTetris.y + 0.7*factor+1;
			eyes[i][0].w=this->dst[i].w*factor*0.15-1;;
			eyes[i][0].h=this->dst[i].h*factor*0.15-1;;

			eyes[i][1].x=(this->dst[i].x + shift)*factor + locTetris.x + 0.6*factor+1;
			eyes[i][1].y=((this->dst[i].y))*factor + locTetris.y + 0.7*factor+1;
			eyes[i][1].w=this->dst[i].w*factor*0.15-1;
			eyes[i][1].h=this->dst[i].h*factor*0.15-1;
		}

		if(src[i].y!=-1) SDL_RenderCopy(renderer, blank, &src_r[i], &src_r[i]);
	}
	if(!erase) {
		for(int i=0; i<4; i++) {
			if(dst[i].y!=-1) {
			//couleur du carré
			if(this->opt & (ACCESS | CLASSIC)) {
				SDL_SetRenderDrawColor(renderer, 20,20,20, alpha);
				if(this->opt & ACCESS)
					SDL_SetRenderDrawColor(renderer, 0,255,4, alpha);

				SDL_RenderDrawRect(renderer, &dst_r[i]);
				SDL_RenderFillRect(renderer, &blanc[i]);
			}
			else {
				SDL_SetRenderDrawColor(renderer, this->color[0], this->color[1], this->color[2], alpha);
				if(this->opt & PASTEL) {
					SDL_SetRenderDrawColor(renderer, this->pastel[0], this->pastel[1], this->pastel[2], alpha);
					SDL_RenderFillRect(renderer, &dst_r[i]);

					SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha*0.15);
					SDL_RenderFillRect(renderer, &dst_r[i]);

				}

				SDL_RenderFillRect(renderer, &dst_r[i]);
				//Carré au milieu pour mettre de la dimension
				SDL_SetRenderDrawColor(renderer, 255, 255,255, alpha*0.1);
				SDL_RenderFillRect(renderer, &blanc[i]);
			}


			//Tour des rectangles
			//en noir
			SDL_SetRenderDrawColor(renderer, 10, 10,10, alpha*0.5);
			//en blanc
			if(this->opt & WHITE_LINED)
				SDL_SetRenderDrawColor(renderer, 255, 255,255, alpha*0.5);


			SDL_RenderDrawRect(renderer,&dst_r[i]);

			if(this->opt & EYES) {
				//yeux (jsp pourquoi)

				SDL_SetRenderDrawColor(renderer, 0, 0,0, alpha*0.9);
				SDL_RenderFillRect(renderer, &eyes[i][0]);
				SDL_RenderFillRect(renderer, &eyes[i][1]);
			}


			//Eclaircissement
			//SDL_SetRenderDrawColor(renderer, 255, 255,255, alpha*0.1);
			//SDL_RenderFillRect(renderer, &dst_r[i]);

			//SDL_SetRenderDrawColor(renderer, 255, 255,255, alpha*0.5);
			//SDL_RenderDrawRect(renderer, &blanc[i]);

			}
		}
	}

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

}
void Piece::mvDstToSrc(Piece &pieceDst){
	for(int i = 0; i < 4; i++) {
		this->src[i].x=pieceDst.dst[i].x;
		this->src[i].y=pieceDst.dst[i].y;
	}
}

bool Piece::translate(int a, int b, bool moveSource){
	if (moveSource)
		this->mvDstToSrc(*this);

	for(int i = 0; i < 4; i++) {
		this->dst[i].x+=a;
		this->dst[i].y+=b;
	}
	return true;
}

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

void Piece::up(bool moveSource){
	this->translate(0, -1, moveSource);
}

void Piece::rotateLeft(bool moveSource){

	Piece temp;
	temp.mvDstToSrc(*this);

	if(moveSource)
		this->mvDstToSrc(*this);

	//ATTENTION, il faut bien séparer les deux boucles !!
	for(int i = 0; i<4; i++) {
		this->dst[i].x = temp.src[i].y - temp.src[1].y + temp.src[1].x;
		this->dst[i].y= temp.src[1].x - temp.src[i].x + temp.src[1].y;
	}
}

void Piece::rotateRight(bool moveSource){

	Piece temp;
	temp.mvDstToSrc(*this);

	if(moveSource)
		this->mvDstToSrc(*this);

	//ATTENTION, il faut bien séparer les deux boucles !!
	for(int i = 0; i<4; i++) {
		this->dst[i].x = temp.src[1].y - temp.src[i].y + temp.src[1].x;
		this->dst[i].y= temp.src[i].x - temp.src[1].x + temp.src[1].y;
	}
}

bool Piece::onDown(bool mat[BLOCSX][BLOCSY], bool cont, SDL_Renderer* renderer,
	SDL_Texture*  blank, SDL_Texture* texture){

	if(this->isLegalDown(mat).OVER_Y
			or this->isLegalDown(mat).COLLISION_PIECE) {
		cont = false;

		for(int i = 0; i < 4; i++)
			mat[this->getx(i)][this->gety(i)]=true;
	}
	else if(this->isLegalDown(mat).NO_ERROR) {
		this->down();
		this->draw(renderer,blank,texture);
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
			//std::cout << "mouvement illégal (dv)" << std::endl;
			e.OVER_Y=true;
			e.NO_ERROR=false;
		}
		//Verification occupation de la case
		if(mat[temp->dst[i].x][temp->dst[i].y]) {
			//std::cout << "mouvement illégal (oc)" << std::endl;
			e.COLLISION_PIECE=true;
			e.NO_ERROR=false;
		}

		if(temp->dst[i].x < 0 || temp->dst[i].x >= BLOCSX) {
			//std::cout << "mouvement illégal (dh)" << std::endl;
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

Error Piece::isLegalRotateLeft(bool mat[BLOCSX][BLOCSY]){

	Piece temp;

	for(int i = 0; i<4; i++) {
		temp.dst[i].x = this->dst[i].y - this->dst[1].y + this->dst[1].x;
		temp.dst[i].y= this->dst[1].x - this->dst[i].x + this->dst[1].y;
	}
	return isLegalPosition(&temp, mat);
}

Error Piece::isLegalRotateRight(bool mat[BLOCSX][BLOCSY]){

	Piece temp;

	for(int i = 0; i<4; i++) {
		temp.dst[i].x = this->dst[1].y - this->dst[i].y + this->dst[1].x;
		temp.dst[i].y= this->dst[i].x - this->dst[1].x + this->dst[1].y;
	}
	return isLegalPosition(&temp, mat);
}

void Piece::affiche_coord(bool source, bool dest){
	if(source) {
		std::cout << "coordonnées de la source "<<std::endl;
		for(int i=0; i< 4; i++) {
			std::cout << "(" <<  this->src[i].x << ", " << this->src[i].y << ")";
			std::cout << "    (taille : (" << this->src[i].w << ",";
			std::cout << this->src[i].h << "))" << std::endl;
		}
	}
	if(dest) {
		std::cout << "coordonnées de la destination"<<std::endl;
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

int Piece::getpastel(int i) {
	return this->pastel[i];
}
void Piece::update(){
}

void Piece::verif(Piece * ref) {
	for(int i = 0; i<4; i++) {
		for(int j = 0; j<4; j++) {
			if(this->dst[i].y==ref->gety(j) && this->dst[i].x==ref->getx(j))
				this->dst[i].y=-1;
			if(this->src[i].y==ref->gety(j) && this->src[i].x==ref->getx(j))
				this->src[i].y=-1;
		}
	}
}

void Piece::adjust(Piece *piece) {
	for(int i = 0; i<4; i++) {
		this->src[i].x=piece->getx(i);
		this->dst[i].x=piece->getx(i);
		this->src[i].y=piece->gety(i);
		this->dst[i].y=piece->gety(i);
	}
	for(int i = 0; i< 3; i++) {
		this->color[i]=piece->getcol(i);
		this->pastel[i]=piece->getpastel(i);
	}
}

void Piece::printinsquare(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture, SDL_Rect* square, int a, int b, bool init, float xShift) {
	int factor = locTetris.w/BLOCSX;

	SDL_SetRenderTarget(renderer, texture);

	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	if(this->opt & CLASSIC) {
		SDL_SetRenderDrawColor(renderer,175,175,135,255);
	}
	else if(this->opt & ACCESS) {
		SDL_SetRenderDrawColor(renderer,10,10,10,255);
	}
	else if(this->opt & PASTEL) {
		SDL_SetRenderDrawColor(renderer,212,255,254,255);
	}
	SDL_RenderFillRect(renderer,square);

	SDL_SetRenderDrawColor(renderer, this->color[0], this->color[1], this->color[2], 255);

	SDL_RenderDrawRect(renderer,square);

	SDL_SetRenderTarget(renderer, NULL);
	if(!init) {
		Piece * temp = new Piece(this->opt);
		temp->adjust(this);

		temp->translate(a, b, false);
		temp->translate(0,0,true);

		temp->draw(renderer, blank, texture, 255, false, xShift);

		delete(temp);
	}

}

void Piece::printNextPiece2(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture){
	SDL_Rect tour;
	int factor = locTetris.w/BLOCSX;

	tour.x = locTetris.w +locTetris.x + factor;
	tour.y = locTetris.h / 3 - 1.5*factor + locTetris.y;
	tour.h = 5*factor;
	tour.w = 4*factor;

	int a =  locTetris.w/(2*factor) +1;
	int b = locTetris.h/(3*factor)+1;
	printinsquare(renderer, blank, texture, &tour, a, b);
}

void Piece::printreserve(SDL_Renderer* renderer, SDL_Texture*  blank,SDL_Texture* texture, bool multiplayer, bool init) {
	SDL_Rect tour;
	int factor = locTetris.w/BLOCSX;

	tour.x = locTetris.x - 5*factor;
	tour.y = locTetris.h / 3 - 1.5*factor + locTetris.y;
	tour.h = 5*factor;
	tour.w = 4*factor;

	int a = -locTetris.w/(2*factor) -5;
	int b = locTetris.h/(3*factor) +1;
	this->affiche_coord(1,1);
	if(multiplayer) {
		tour.x += locTetris.w + 6.1*factor + tour.w;
		//tour.y += tour.h + 0.5*factor;
		a+= (locTetris.w+tour.w)/factor+6;
		//b+= tour.h/factor + 0.5;
	}
	printinsquare(renderer, blank, texture, &tour, a, b,init);
}


void Piece::DownGhost(bool mat[BLOCSX][BLOCSY],Piece * ref, bool gen) {
	Piece temp = *ref;

	while (temp.isLegalDown(mat).NO_ERROR)
		for (int i=0 ; i <4 ; i++)
			temp.dst[i].y ++;

	for (int i=0 ; i <4 ; i++){
			this->src[i].y = this->dst[i].y;
			this->src[i].x = this->dst[i].x;
			this->dst[i].x = temp.dst[i].x;
			this->dst[i].y = temp.dst[i].y;
	}

	if(gen)
		this->mvDstToSrc(*this);

}

void Piece::holdPiece(bool mat[BLOCSX][BLOCSY]){

	Error e;

	Piece temp = *this;
	while (temp.isLegalDown(mat).NO_ERROR)
		for (int i=0 ; i <4 ; i++) temp.dst[i].y += 1;

	for (int i=0 ; i <4 ; i++){
		this->src[i].y = this->dst[i].y;
		this->src[i].x = this->dst[i].x;
		this->dst[i].y = temp.dst[i].y;
	}

	for(int i = 0; i < 4; i++)
		mat[this->getx(i)][this->gety(i)]=true;

}

int Piece::nbFullLine(bool mat[BLOCSX][BLOCSY]){
	int decalage = 0;
	for(int i = BLOCSY-1; i>=0; i--) {
		int compt = 0;
		for(int j = 0; j<BLOCSX; j++) {
			if(mat[j][i]) compt++;
		}
		if(compt==BLOCSX) //ligne pleine
		{
			decalage++;
		}
	}
	return decalage;
}

void Piece::cheat(bool mat[BLOCSX][BLOCSY]){

	//define some functions. This functions are useful to calculate
	//the score of a position piece
	auto sumHauteur = [&] (Piece best_piece)
	{
		int sumHauteur = 0;
		for (int i=0; i<4; i++)
			sumHauteur += BLOCSY - best_piece.dst[i].y ;
		return sumHauteur;
	};

	//return the number of "trou" in a matrice
	auto nbTrou = [&] ( bool matTemp[BLOCSX][BLOCSY]){
		int nbTrou =0;
		for (int i= 0; i< BLOCSX; i++){
			for (int j= 0; j< BLOCSY; j++){
				if (matTemp[i][j]){
					while(j+1 < BLOCSY and !matTemp[i][j+1]){
						nbTrou+=1;
						j++;
					}
				}
			}
		}
		return nbTrou;
	};

	auto diffHauteurs = [&] ( bool matTemp[BLOCSX][BLOCSY]){
		int totDiff = 0;

		int maxHauteur[BLOCSX];
		for (int i=0; i<BLOCSX; i++){
			maxHauteur[i]=0;
			int j=0;
			while(j<BLOCSY and !matTemp[i][j]){
				maxHauteur[i]=BLOCSY - j;
				j++;
			}
		}

		for(int i=0; i<BLOCSX-1; i++){
			if (maxHauteur[i+1]-maxHauteur[i] >=0)
				totDiff+=(maxHauteur[i+1]-maxHauteur[i]+1);
			else
				totDiff+=(maxHauteur[i]-maxHauteur[i+1]+1);
		}
		return totDiff;
	};

	//on déplace la dest dans la source pour  draw
	this->mvDstToSrc(*this);
	bool matTemp[BLOCSX][BLOCSY];

	//get minimum x of a piece
	int minX =40;
	for (auto & rect : this->dst )
		minX = rect.x < minX ? rect.x : minX;

	Piece best_piece = *this, init_piece = *this;
	for (int i =0 ; i<4; i++)
		init_piece.dst[i].x = this->dst[i].x - minX;

	int bestScore = -10000, rotation = 0, score =0;
	while(rotation < 5){

		for(int step =0; step< BLOCSX; step++){

			score = 0;
			// on réinitialise matTemp
			for (int i= 0; i< BLOCSX; i++)
				for (int j= 0; j< BLOCSY; j++)
					matTemp[i][j] = mat[i][j];

			//on décale best_piece
			for (int i = 0; i<4 ; i++){
				best_piece.dst[i].x = init_piece.dst[i].x +step;
				best_piece.dst[i].y = init_piece.dst[i].y +2 ;
			}

			//on vérifie que la position est légale avant dutiliser holsPiece
			if (best_piece.isLegalPosition(&best_piece, matTemp).NO_ERROR){
				best_piece.holdPiece(matTemp);
				score += best_piece.nbFullLine(matTemp)*30;
				score -= sumHauteur(best_piece);
				score -= nbTrou(matTemp) * 6 ;
				score -= diffHauteurs(mat);

				if (score > bestScore){
					for (int i= 0; i<4; i++){
						this->dst[i].x = best_piece.dst[i].x;
						this->dst[i].y = best_piece.dst[i].y;
					}
					bestScore = score;
				}
			}
		}
		init_piece.rotateRight();
		rotation++;
	}
	//update matrice
	//for(int i = 0; i < 4; i++)
	//	mat[this->getx(i)][this->gety(i)]=true;
}
/*############################################################################
 ########################          LEFT L         #############################
 ############################################################################*/

JTetri::JTetri(unsigned int options ) : Piece(options){
	this->opt = options;

	this->color[0]=0;
	this->color[1]=90;
	this->color[2]=157;

	this->pastel[0]=160;
	this->pastel[1]=196;
	this->pastel[2]=255;
}

void JTetri::update() {
	this->src[0].x=floor(BLOCSX/2)+1;
	this->src[1].x=floor(BLOCSX/2);
	this->src[2].x=floor(BLOCSX/2);
	this->src[3].x=floor(BLOCSX/2);

	this->src[0].y=0;
	this->src[1].y=0;
	this->src[2].y=1;
	this->src[3].y=2;

	this->dst[0].x=floor(BLOCSX/2)+1;
	this->dst[1].x=floor(BLOCSX/2);
	this->dst[2].x=floor(BLOCSX/2);
	this->dst[3].x=floor(BLOCSX/2);

	this->dst[0].y=0;
	this->dst[1].y=0;
	this->dst[2].y=1;
	this->dst[3].y=2;
}

LTetri::LTetri(unsigned int options) : Piece(){
	this->opt = options;

	this->color[0]=248;
	this->color[1]=150;
	this->color[2]=34;

	this->pastel[0]=255;
	this->pastel[1]=214;
	this->pastel[2]=168;
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

void LTetri::placeprev(int x, int y) {
	for(int i = 0; i<3; i++) {
		this->src[i].x= x;
		this->src[i].y=y+i+1;

		this->dst[i].x=x;
		this->dst[i].y=y+i+1;
	}

	this->src[3].x=x+1;
	this->src[3].y= y+1;
	this->dst[3].x=x+1;
	this->dst[3].y= y+1;
}

OTetri::OTetri(unsigned int options) : Piece() {
	this->opt = options;

	this->color[0]=253;
	this->color[1]=225;
	this->color[2]=0;

	this->pastel[0]=253;
	this->pastel[1]=255;
	this->pastel[2]=182;
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

Error OTetri::isLegalRotateRight(bool mat[BLOCSX][BLOCSY]){
	Error e;
	e.NO_ERROR = true;
	return e;
}

void OTetri::rotateRight(bool moveSource){
}

void OTetri::printinsquare(SDL_Renderer* renderer,
		SDL_Texture*  blank,SDL_Texture* texture,SDL_Rect* square,
		int a, int b, bool init,float xShift){
	this->Piece::printinsquare(renderer, blank, texture,square,a,b, init,0.9);
}

ITetri::ITetri(unsigned int options) : Piece() {
	this->opt = options;

	this->color[0]=43;
	this->color[1]=172;
	this->color[2]=226;

	this->pastel[0]=155;
	this->pastel[1]=246;
	this->pastel[2]=255;
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

void ITetri::printinsquare(SDL_Renderer* renderer,
		SDL_Texture*  blank,SDL_Texture* texture, SDL_Rect* square, int a, int b, bool init, float xShift){
	Piece temp = *this;
	temp.dst[0].x = floor(BLOCSX/2)+1;
	temp.dst[1].x = floor(BLOCSX/2)+1;
	temp.dst[2].x = floor(BLOCSX/2)+1;
	temp.dst[3].x = floor(BLOCSX/2)+1;

	temp.dst[0].y = -1;
	temp.dst[1].y = 0;
	temp.dst[2].y = 1;
	temp.dst[3].y = 2;


	temp.Piece::printinsquare(renderer, blank, texture,square,a,b,init, xShift);
}

TTetri::TTetri(unsigned int options) : Piece() {
	this->opt = options;

	this->color[0]=146;
	this->color[1]=43;
	this->color[2]=140;

	this->pastel[0]=189;
	this->pastel[1]=178;
	this->pastel[2]=255;

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

ZTetri::ZTetri(unsigned int options) : Piece() {
	this->opt = options;

	this->color[0]=238;
	this->color[1]=39;
	this->color[2]=51;

	this->pastel[0]=255;
	this->pastel[1]=190;
	this->pastel[2]=190;
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


STetri::STetri(unsigned int options) : Piece() {
	this->opt = options;

	this->color[0]=78;
	this->color[1]=183;
	this->color[2]=72;

	this->pastel[0]=202;
	this->pastel[1]=255;
	this->pastel[2]=192;
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
