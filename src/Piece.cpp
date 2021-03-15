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

void Piece::down(double &timer, SDL_Renderer* renderer){
	
	this->src.x=this->dst.x;
	this->src.y=this->dst.y; 
	this->src.w=this->dst.w;
	this->src.h=this->dst.h;
	
	this->dst.y+=40;
	//SDL_RenderFillRect(renderer, dest);
	
	timer=timer+40;
	
	printf("%f ",timer);
	//SDL_Rect rect = {40, 40, 80, 80};
	
	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	SDL_RenderFillRect(renderer, &this->src);
	
	SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255); /* On dessine en violet */
	
	//SDL_RenderDrawRect(renderer, &rect);//Pour dessiner un seul rectangle
	
	//SDL_SetRenderTarget(renderer, texture); /* On va dessiner sur la texture */
	SDL_RenderFillRect(renderer, &this->dst);
	
	//SDL_SetRenderTarget(renderer, NULL);
	//SDL_RenderCopy(renderer, texture, &src, &dest);
	
	SDL_RenderPresent(renderer);
}

void Piece::right(double &timer, SDL_Renderer* renderer){
	
	this->src.x=this->dst.x;
	this->src.y=this->dst.y; 
	this->src.w=this->dst.w;
	this->src.h=this->dst.h;
	
	this->dst.x+=40;
	//this->dst.x+=0.001;
	//SDL_RenderFillRect(renderer, dest);
	
	timer=timer+40;
	
	printf("%f ",timer);
	//SDL_Rect rect = {40, 40, 80, 80};
	
	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	SDL_RenderFillRect(renderer, &this->src);
	
	SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255); /* On dessine en violet */
	
	//SDL_RenderDrawRect(renderer, &rect);//Pour dessiner un seul rectangle
	
	//SDL_SetRenderTarget(renderer, texture); /* On va dessiner sur la texture */
	SDL_RenderFillRect(renderer, &this->dst);
	
	//SDL_SetRenderTarget(renderer, NULL);
	//SDL_RenderCopy(renderer, texture, &src, &dest);
	
	SDL_RenderPresent(renderer);
}

void Piece::left(double &timer, SDL_Renderer* renderer){
	
}