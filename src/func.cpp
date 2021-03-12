/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <SDL.h>
#include <string>
#include <iostream>
#include <cassert>

#include "../include/func.hpp"

Func::Func() {
    pWindow = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            600, 600, SDL_WINDOW_SHOWN);
    
}



Func::~Func() { 
}

void Func::createPiece(SDL_Renderer* renderer){
    //coloriage d'un petit rectangle en rouge
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // the rect color (solid red)
    SDL_Rect rect; // the rectangle
    rect.x=50;
    rect.y=50;
    rect.w=20;
    rect.h=20;
    SDL_RenderFillRect(renderer, &rect);
}