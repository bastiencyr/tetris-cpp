/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#ifndef _FUNC_HPP_
#define _FUNC_HPP_

#include <SDL.h>
#include <string>

//class Sprite;

class Func
{
public:
    
    SDL_Window* pWindow;
    //SPrite 

public :
    void createPiece(SDL_Renderer* renderer);
    Func();
    ~Func(); // destructor
    
    
};

#endif