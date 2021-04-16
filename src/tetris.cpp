/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cassert>
#include <time.h>
#include <SDL_image.h>
#include "../include/tetris.hpp"
#include <SDL2/SDL_ttf.h>
#include "../include/Error.hpp"
#include "../include/Player.hpp"

#define OPAC 70
#define FREE_SURFACE(surface_t) { SDL_FreeSurface(surface_t); surface_t = nullptr;}
#define FREE_TEXTURE(texture_t) { SDL_DestroyTexture(texture_t); texture_t = nullptr;}
#define FREE_RENDERER_AND_WINDOW(renderer_t, window_t) { \
SDL_DestroyRenderer(renderer_t); SDL_DestroyWindow(window_t); \
renderer_t = nullptr; window_t = nullptr;}


Tetris::Tetris(int w, int h, SDL_Rect locTetris, SDL_Renderer* &renderer, bool multiplayer){
	this->w=w;
	this->h=h;
	this->options = 0x0;
	timer=0;
	this->renderer = renderer;
	quitgame = true;
	this-> volume = MIX_MAX_VOLUME/2;

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	blank = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	menu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);

	sizeTetris.w = locTetris.w;
	sizeTetris.h = sizeTetris.w*2;
	sizeTetris.x = locTetris.x;
	sizeTetris.y = locTetris.y;


	sizeTetris2.w = locTetris.w;
	sizeTetris2.h = sizeTetris.w*2;
	sizeTetris2.x = sizeTetris2.w; //+ size_bloc * 7;
	sizeTetris2.y = locTetris.y;
}

Tetris::~Tetris(){
	FREE_TEXTURE(blank);
	//FREE_TEXTURE(texture);
	//FREE_TEXTURE(menu);
	//FREE_RENDERER_AND_WINDOW(renderer, pWindow);

}

void Tetris::init(Mix_Music* music, bool multiplayer){


	if (Mix_PlayingMusic() == 0) Mix_PlayMusic(music,-1);

	int size_bloc = sizeTetris.w/BLOCSX;
	if (multiplayer){
		sizeTetris.x = 0;
		sizeTetris2.x = 2 * sizeTetris.w ;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_Point ligne_depart,ligne_arrivee; // Déclaration du point de départ et du point d'arrivée d'une ligne

	//on colorie le fond
	SDL_SetRenderTarget(renderer, blank);

	SDL_SetRenderDrawColor(renderer,63,63,63,255);
	if(options&CLASSIC) {
		SDL_SetRenderDrawColor(renderer,175,175,135,255);
	}
	else if(options&ACCESS) {
		SDL_SetRenderDrawColor(renderer,10,10,10,255);
	}
	else if(options&PASTEL) {
		SDL_SetRenderDrawColor(renderer,212,255,254,255);
		//SDL_SetRenderDrawColor(renderer,10,10,10,255);
	}
	SDL_RenderClear(renderer);

	//Les lignes
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	if(options&ACCESS) SDL_SetRenderDrawColor(renderer, 0,255,4, 100);
	else if(options&PASTEL) SDL_SetRenderDrawColor(renderer,0,0,0,80);


	// Lignes horizontales
	ligne_depart.x = sizeTetris.x;
	ligne_arrivee.x = w + sizeTetris.x; //h
	ligne_depart.y = sizeTetris.y ;

	while(ligne_depart.y<sizeTetris.h + sizeTetris.y ) //h
	{
		ligne_depart.y += size_bloc;
		ligne_arrivee.y = ligne_depart.y;
		SDL_RenderDrawLine(renderer,ligne_depart.x, ligne_depart.y,ligne_arrivee.x,ligne_arrivee.y);
	}

	// Lignes verticales
	ligne_depart.x = sizeTetris.x;
	ligne_depart.y = sizeTetris.y ;
	ligne_arrivee.y = sizeTetris.h + sizeTetris.y ; //h

	while(ligne_depart.x< w + sizeTetris.x ){ //h
		ligne_depart.x += size_bloc;
		ligne_arrivee.x = ligne_depart.x;
		SDL_RenderDrawLine(renderer,ligne_depart.x, ligne_depart.y,ligne_arrivee.x,ligne_arrivee.y);
	}

	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, blank, &sizeTetris, &sizeTetris);

	if(multiplayer)
		SDL_RenderCopy(renderer, blank, &sizeTetris, &sizeTetris2);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, &sizeTetris, &sizeTetris);

	if(multiplayer)
		SDL_RenderCopy(renderer, texture, &sizeTetris, &sizeTetris2);

}

ReturnCodeMenu Tetris::loop(Mix_Music* music, bool multiplayer){


	auto ghostVerifDraw = [&] (Piece *ghost, Piece *piece, bool matGame[BLOCSX][BLOCSY], bool gen =false)
	{
		ghost->DownGhost(matGame,piece,gen);
		ghost->verif(piece);
		ghost->draw(renderer,blank,texture,OPAC);
	};

	Piece::initStaticMembers(sizeTetris);

	Player player1 (renderer, texture, blank, sizeTetris, options);
	Player ghostPlayer1 (renderer, texture,blank, sizeTetris, options);
	Player player2 (renderer, texture,blank, sizeTetris2, options);

	Mix_Music* rotate = Mix_LoadMUS("sfx/SFX_PieceRotateLR.ogg");
	Mix_Music* drop = Mix_LoadMUS("sfx/SFX_PieceSoftDrop.ogg");
	Mix_Music* line = Mix_LoadMUS("sfx/SFX_SpecialLineClearSingle.ogg");
	Mix_Music* lines = Mix_LoadMUS("sfx/SFX_SpecialLineClearTriple.ogg");

	//INITIALISATION
	if (Mix_PlayingMusic() == 0) Mix_PlayMusic(music,-1);
	Uint64 prev, now = SDL_GetPerformanceCounter(); // timers

	bool quit_loop = false;
	ReturnCodeMenu gameState = ReturnCodeMenu::INIT ;

	bool cont = true, win = true;
	double t=0, delta_t=0, tIA =0, delta_tIA = 0;
	int scoreOldIA = 1, ScoreOld=-1;

	if (multiplayer){
		player2.printScoreText(1.6 * sizeTetris.w, sizeTetris.h/15);
		player1.printScoreText(1.1 * sizeTetris.w, sizeTetris.h/15);
		player2.printScore(1.6 * sizeTetris.w, sizeTetris.h/15);
		player1.printScore(1.1 * sizeTetris.w, sizeTetris.h/15);
		player1.printSeparation();
	}
	else{
		player1.printScoreText(0.6 * sizeTetris.w, sizeTetris.h/15);
		player1.printScore(0.6 * sizeTetris.w, sizeTetris.h/15);
	}
	Piece *piece, *newPiece, *pieceIA, *ghost, *reserve, *temp;
	reserve = nullptr; temp = nullptr;
	//player1
	newPiece = player1.getRandomPiece();
	piece = player1.getRandomPiece();
	player1.nouvPiece(piece, newPiece) ;

	for(int i = 0; i<7; i++)
		ghostPlayer1.getPiece(i)->adjust(player1.getPiece(i));

	ghost=ghostPlayer1.getPiece(0);
	ghost->adjust(piece);
	ghost->DownGhost(player1.matGame,piece,1);
	ghost->draw(renderer,blank,texture,OPAC);
	piece->printreserve(renderer, blank, texture, multiplayer,true);

	int sizeBloc = sizeTetris.w/BLOCSX;
	//BOUCLE
	while (!quit_loop)
	{
		if (Mix_PlayingMusic() == 0) Mix_PlayMusic(music,-1);

		if(!cont) {
			gameState = player1.nouvPiece(piece, newPiece);
			if(gameState == ReturnCodeMenu::GAME_OVER){
				win = false;
				quit_loop = true;
			}

			cont = true;
			/*
			//Mix_PlayMusic(drop, 0);
			*/
			ghost->adjust(piece);
			ghostVerifDraw(ghost, piece, player1.matGame, true);
		}

		SDL_Event event;
		while (!quit_loop && SDL_PollEvent(&event) && cont == true)
		{
			switch (event.type)
			{

			case SDL_QUIT:
				quit_loop = true;
				gameState = ReturnCodeMenu::QUIT_GAME;
				break;

			case SDL_MOUSEBUTTONDOWN:
				break;

			case SDL_KEYDOWN:
				/* Check the SDLKey values and move change the coords */
				switch( event.key.keysym.sym ){

				case SDLK_SPACE:
					piece->holdPiece(player1.matGame);
					piece->draw(renderer,blank,texture);
					cont = false;
					break;

				case SDLK_F1:
					piece->cheat(player1.matGame);
					for(int i = 0; i < 4; i++)
						player1.matGame[piece->getx(i)][piece->gety(i)]=true;
					piece->draw(renderer,blank,texture);
					cont = false;
					break;

				case SDLK_r:
					piece->translate(0,0,true);
					ghost->translate(0,0,true);
					piece->draw(renderer, blank, texture,255,true);
					ghost->draw(renderer, blank, texture,255,true);
					if(reserve==nullptr) {
						reserve=piece;
						cont = false;
					}
					else {
						temp = reserve;
						reserve = piece;
						piece = temp;
						temp = nullptr;
						piece->update();
						ghost->adjust(piece);
						ghostVerifDraw(ghost, piece, player1.matGame, true);
					}
					reserve->update();
					reserve->printreserve(renderer, blank, texture, multiplayer);
					break;

				case SDLK_ESCAPE:
					gameState = this->printMenu();
					if (gameState == ReturnCodeMenu::QUIT_GAME
							or gameState == ReturnCodeMenu::GO_TO_MAIN_MENU)
						quit_loop = true;
					if (gameState == ReturnCodeMenu::RESTART ){
						piece->printreserve(renderer, blank, texture, multiplayer,true);
						player1.restart();
						player2.restart();
						if (multiplayer){
							player2.printScore(1.6 * sizeTetris.w, sizeTetris.h/15);
							player1.printScore(1.1 * sizeTetris.w, sizeTetris.h/15);
						}
						else
							player1.printScore(0.6 * sizeTetris.w, sizeTetris.h/15);
					}
					break;

				case SDLK_RIGHT:
					if (piece->isLegalRight(player1.matGame).NO_ERROR){
						piece->right();
						piece->draw(renderer,blank,texture);
						//Mix_PlayMusic(rotate, 0);

						ghostVerifDraw(ghost, piece,  player1.matGame);
					}
					break;

				case SDLK_LEFT:
					if (piece->isLegalLeft(player1.matGame).NO_ERROR){
						piece->left();
						piece->draw(renderer,blank,texture);
						//Mix_PlayMusic(rotate, 0);

						ghostVerifDraw(ghost, piece,  player1.matGame);
					}
					break;

				case SDLK_DOWN:
					cont = piece->onDown(player1.matGame, cont, renderer, blank,
							texture);
					break;

				case SDLK_UP:
					if (piece->isLegalRotateRight(player1.matGame).NO_ERROR){
						piece->rotateRight();
						piece->draw(renderer,blank,texture);

					}
					else if (piece->isLegalRotateRight(player1.matGame).OVER_X){
						Piece temp = *piece;
						int shift = -piece->isLegalRotateRight(player1.matGame).OVER_NUMBER_X;
						temp.rotateRight();
						if (temp.isLegalTranslate(shift, 0, player1.matGame).NO_ERROR){
							piece->rotateRight();
							piece->translate(shift, 0, false);
							piece->draw(renderer,blank,texture);
						}

					}
					ghostVerifDraw(ghost, piece,  player1.matGame);
					break;
				}

			default: break;
			}

			//this->printMatrice();
		}

		prev = now;
		now = SDL_GetPerformanceCounter();
		delta_t = static_cast<double>((now - prev)/
				static_cast<float>(SDL_GetPerformanceFrequency()));

		t+=delta_t;
		tIA += delta_t;
		if(t>=player1.getCurrentDifficulty()) {
			cont = piece->onDown(player1.matGame, cont, renderer,
					blank,texture);
			t=0;
		}


		if (multiplayer and tIA >=2){
			pieceIA = player2.getRandomPiece();
			pieceIA->update();
			pieceIA->cheat(player2.matGame);

			if (!pieceIA->isLegalPosition(pieceIA, player2.matGame).NO_ERROR){
				quit_loop = true;
				gameState = ReturnCodeMenu::GAME_OVER;
				pieceIA->affiche_coord(true, true);
			}
			for (int i=0; i<4; i++){
				player2.matGame[pieceIA->getx(i)][pieceIA->gety(i)]=true;
			}
			pieceIA->draw(renderer,blank,texture, 255, false,
					sizeTetris2.x/sizeBloc);

			tIA = 0;
		}

		int d=player1.tetrisLinesUpdate();
		if (!multiplayer and d>0){
			player1.updateLevel(ScoreOld);
			player1.printScore(0.6 * sizeTetris.w, sizeTetris.h/15);
		}
		//if(d==1) Mix_PlayMusic(line, 0);
		//else if(d>1) Mix_PlayMusic(lines, 0);

		//update score
		if (multiplayer){
			int dia = player2.tetrisLinesUpdate();
			if(dia>=1){
				if(dia==4) player1.addLineToPlayer(dia, pieceIA, ghost, true);
				else player1.addLineToPlayer(dia-1, pieceIA, ghost, true);				player2.updateLevel(scoreOldIA);
				player2.printScore(1.6 * sizeTetris.w, sizeTetris.h/15);
			}

			if (d >= 1){
				player1.updateLevel(ScoreOld);
				if(d==4) player2.addLineToPlayer(d, pieceIA, ghost, true);
				else player2.addLineToPlayer(d-1, pieceIA, ghost, true);
				player1.printScore(1.1 * sizeTetris.w, sizeTetris.h/15);
			}
		}

		SDL_RenderPresent(renderer);
	}

	//this->printMenu();
	Mix_FreeMusic(rotate);
	Mix_FreeMusic(drop);
	Mix_FreeMusic(line);
	Mix_FreeMusic(lines);

	if (gameState == ReturnCodeMenu::GAME_OVER)
		gameState = this->endGameMenu( music, multiplayer, win);

	return gameState;
}


void Tetris::DrawSelected() {
	int sizeBetweenText=40;
	SDL_Rect checkbox = {w/2+100, h/2 - 60/2, 20, 20};
	int mode=0;
	unsigned int modes[3] = {CLASSIC,ACCESS,PASTEL};
	SDL_SetRenderDrawColor(renderer,150,150,150,255);
	SDL_RenderFillRect(renderer, &checkbox);
	for(int i = 0; i< 3; i++) {
		checkbox.y+= sizeBetweenText;
		SDL_SetRenderDrawColor(renderer,150,150,150,255);
		SDL_RenderFillRect(renderer, &checkbox);
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		SDL_RenderDrawRect(renderer, &checkbox);
		if(this->options&modes[i]) {
			SDL_RenderDrawLine(renderer,checkbox.x,checkbox.y, checkbox.x+checkbox.w, checkbox.y+checkbox.h);
			SDL_RenderDrawLine(renderer,checkbox.x, checkbox.y+checkbox.h, checkbox.x+checkbox.w,checkbox.y);
			mode++;
		}
	}
	if(mode==0) {
		checkbox.y -= sizeBetweenText*3;
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		SDL_RenderDrawRect(renderer, &checkbox);
		SDL_RenderDrawLine(renderer,checkbox.x,checkbox.y, checkbox.x+checkbox.w, checkbox.y+checkbox.h);
		SDL_RenderDrawLine(renderer,checkbox.x, checkbox.y+checkbox.h, checkbox.x+checkbox.w,checkbox.y);
	}
	SDL_RenderPresent(renderer);
}

void Tetris::minimenu(SDL_Texture * menu, SDL_Rect * cadre) {
	SDL_Texture * temp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_Rect rectcop = {w/2-125, h/2 - (3*80)/2, 250, 160};
	SDL_Rect texte = {cadre->x, cadre->y, cadre->w-50, cadre->h};
	SDL_SetRenderTarget(renderer, temp);
	SDL_RenderCopy(renderer, menu, NULL, NULL);
	SDL_SetRenderTarget(renderer, NULL);

	TTF_Font * police = TTF_OpenFont("src/RetroGaming.ttf", 65);
	if(!police){
		std::cout << TTF_GetError()<< std::endl;
	}
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_Color textColor = {255, 255, 255};
	const char * modes[3] = {"Classic", "Accessible", "Pastel"};
	//write first option
	SDL_Surface * text_surface = TTF_RenderText_Blended(police,"Defaut", textColor);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_RenderFillRect(renderer, cadre);
	SDL_RenderCopy(renderer, text_texture, NULL, &texte);

	FREE_SURFACE(text_surface);
	FREE_TEXTURE(text_texture);

	for(int i = 0; i<3; i++) {
		text_surface = TTF_RenderText_Blended(police,modes[i], textColor);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		cadre->y += 40;
		texte.y += 40;
		SDL_RenderFillRect(renderer, cadre);
		SDL_RenderCopy(renderer, text_texture, NULL, &texte);
		FREE_SURFACE(text_surface);
		FREE_TEXTURE(text_texture);
	}
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	cadre->y -= 40*3;
	SDL_RenderDrawRect(renderer, cadre);
	SDL_RenderPresent(renderer);

	DrawSelected();
	int choiceMenu = 0;
	int numberChoice = 4, sizeBetweenText = 40, xShift = 100;;
	bool quit_menu = false;
	SDL_Event event;


	while (!quit_menu && SDL_WaitEvent(&event)){
		switch (event.type)
		{
		case SDL_QUIT:
			quit_menu = true;
			break;

		case SDL_KEYDOWN:

			switch( event.key.keysym.sym ){

			case SDLK_DOWN:
				this->UpDownCasesLoopMenu(0,1, choiceMenu, numberChoice, sizeBetweenText, xShift, *cadre);
				break;

			case SDLK_UP:
				this->UpDownCasesLoopMenu(0,0, choiceMenu, numberChoice, sizeBetweenText, xShift, *cadre);
				break;

			case SDLK_RETURN:
				if (choiceMenu == 0){
					setmodedefault();
					DrawSelected();
				}

				else if (choiceMenu == 1){
					setmode(CLASSIC);
					DrawSelected();
				}


				else if (choiceMenu == 2){
					setmode(ACCESS);
					DrawSelected();
				}

				else if (choiceMenu == 3){
					setmode(PASTEL);
					DrawSelected();
				}
				quit_menu = true;
				break;

			case SDLK_ESCAPE:
				quit_menu = true;
				break;

			default:
				break;
			}

		default :break;
		}
	}
	TTF_CloseFont(police);
	SDL_RenderCopy(renderer, temp, NULL, NULL);
	SDL_RenderPresent(renderer);
	FREE_TEXTURE(temp);
}

void Tetris::addmenuoptions(SDL_Texture * menu, int xShift, int sizeBetweenText,
	int numberChoice, int indice, int numItem,const char * str1, const char * str2) {
		TTF_Font * police = TTF_OpenFont("src/RetroGaming.ttf", 65);
		if(!police){
			std::cout << TTF_GetError()<< std::endl;
		}
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_Color textColor = {255, 255, 255};
		SDL_Rect texte= { w/2+sizeBetweenText+xShift+25, h/2 - ((numberChoice-2*indice) * sizeBetweenText)/2, 2*xShift, 40};
		SDL_Rect cadre = { w/2+sizeBetweenText+xShift+25, h/2 - ((numberChoice-2*indice) * sizeBetweenText)/2, 2*xShift+50, 40};
		//write first option
		SDL_Surface * text_surface = TTF_RenderText_Blended(police,str1, textColor);
		SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_RenderFillRect(renderer, &cadre);
		SDL_RenderCopy(renderer, text_texture, NULL, &cadre);

		if(numItem>1) {
			FREE_SURFACE(text_surface);
			FREE_TEXTURE(text_texture);
			cadre.x -= 2*(sizeBetweenText + 2*xShift+50);
			texte.x -= 2*(sizeBetweenText + 2*xShift+50);
			text_surface = TTF_RenderText_Blended(police,str2, textColor);
			text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
			SDL_RenderFillRect(renderer, &cadre);
			SDL_RenderCopy(renderer, text_texture, NULL, &texte);
		}
		FREE_SURFACE(text_surface);
		FREE_TEXTURE(text_texture);
		//SDL_SetRenderTarget(renderer, menu);
		SDL_RenderCopy(renderer, menu, NULL, NULL);
		TTF_CloseFont(police);
}

bool Tetris::printGenericMenu(SDL_Texture * menu, int xShift,
		int sizeBetweenText, bool retour, int numItem,...) {

	TTF_Font *policetetris = TTF_OpenFont("src/Tetris.ttf", 65);
	if(!policetetris){
		std::cout << TTF_GetError() << std::endl;
	}

	TTF_Font * police = TTF_OpenFont("src/RetroGaming.ttf", 65);
	if(!police){
		std::cout << TTF_GetError() << std::endl;
	}

	SDL_Color textColor = {255, 255, 255};

	va_list valist;
	va_start(valist, numItem); //initialize valist for num number of arguments
	int numberChoice = numItem--;
	//write title
	SDL_Surface * text_surface = TTF_RenderText_Blended(policetetris,va_arg(valist, char *), textColor);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_SetRenderTarget(renderer, menu);

	SDL_Rect dstrect = { w/2-xShift, 0, 2*xShift, 40 };
	dstrect.y = h/2 - (numberChoice * sizeBetweenText)/2;
	SDL_Rect cadrect = { w/2-xShift-25, h/2 - (numberChoice * sizeBetweenText)/2, 2*xShift+50, 40};
	SDL_Rect cadre = { w/2-xShift-25, h/2 - (numberChoice * sizeBetweenText)/2+sizeBetweenText, 2*xShift+50, 40};

	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderDrawLine(renderer,w/2-xShift-25,h/2 - (numberChoice * sizeBetweenText)/2-10, w/2+xShift+25, h/2 - (numberChoice * sizeBetweenText)/2-10);
	SDL_RenderDrawLine(renderer,w/2-xShift-25,h/2 - (numberChoice * sizeBetweenText)/2+50, w/2+xShift+25, h/2 - (numberChoice * sizeBetweenText)/2+50);

	//on copie le text sur le menu
	//on dessine le cadre du texte

	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	FREE_SURFACE(text_surface);
	FREE_TEXTURE(text_texture);

	auto printTextBellow = [&] (const char * str,SDL_Surface *text_surface,
			SDL_Texture *text_texture,SDL_Rect &dstrect,SDL_Rect &cadrect)
	{
		text_surface = TTF_RenderText_Blended(police,str, textColor);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		dstrect.y += sizeBetweenText;
		cadrect.y += sizeBetweenText;
		SDL_RenderFillRect(renderer, &cadrect);
		SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
		FREE_SURFACE(text_surface);
		FREE_TEXTURE(text_texture);
	};

	dstrect.y = h/2 - (numberChoice * sizeBetweenText)/2;
	cadrect.y = dstrect.y;

	while(numberChoice > retour){
		printTextBellow(va_arg(valist, char *),
				text_surface, text_texture, dstrect, cadrect);
		numberChoice--;
	}

	if(retour) {
		text_surface = TTF_RenderText_Blended(police,"Retour", textColor);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		dstrect.x = 3*w/4-20;
		cadrect.x = 3*w/4-45;
		dstrect.y = 7*h/8;
		cadrect.y = 7*h/8;
		SDL_RenderFillRect(renderer, &cadrect);
		SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
		FREE_SURFACE(text_surface);
		FREE_TEXTURE(text_texture);
	}

	//on dessine le cadre du premier texte
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderDrawRect(renderer, &cadre);

	va_end(valist); //clean memory reserved for valist

	SDL_RenderCopy(renderer, menu, NULL, NULL);
	TTF_CloseFont(police);
	TTF_CloseFont(policetetris);

	return true;
}

void Tetris::UpDownCasesLoopMenu(int retour, int way, int & choiceMenu ,
	int numberChoice, int sizeBetweenText, int xShift,SDL_Rect & cadre, bool vol) {

	//way = 1 is down
	if(way) {
		choiceMenu+=1;

	}
	else {
		choiceMenu-=1 - numberChoice;
	}
	choiceMenu = choiceMenu % numberChoice ;

	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderDrawRect(renderer, &cadre);

	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	cadre.y = h/2 - ((numberChoice) * sizeBetweenText)/2
			+ (choiceMenu+1) * sizeBetweenText ;
	if(retour) {
		if(choiceMenu==numberChoice-1) {
			cadre.y =7*h/8 ;
			cadre.x = 3*w/4-45;
		}
		else
			cadre.x = w/2-xShift-25;
	}
	if(vol) {
		if(choiceMenu==0) {
			cadre.x = w/2-100-25;
			cadre.y =h/2+50;
			cadre.h = 10;
		}
		else {
			cadre.h=40;
			cadre.w=2*xShift+50;
		}

	}
	SDL_RenderDrawRect(renderer, &cadre);
	SDL_RenderPresent(renderer);
}

ReturnCodeMenu Tetris::endGameMenu(Mix_Music* music, bool multiplayer, bool win){
	int numberChoice = 3;
	int sizeBetweenText = 80, xShift = 100;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//on copie la texture de fond sur le menu
	SDL_SetRenderTarget(renderer, menu);
	SDL_SetRenderDrawColor(renderer,63,63,63,200);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderFillRect(renderer, NULL);
	const char * str;
	if(multiplayer && win)  str = "Partie gagnee";
	else str = "Game Over";
	printGenericMenu(menu, xShift, sizeBetweenText ,false, numberChoice, str, "Recommencer",
			"Aller au menu", "Quitter");

	SDL_Rect cadre ={w/2-xShift-25,0, 250, 40};
	cadre.y = sizeTetris.h/2 - (numberChoice * sizeBetweenText)/2 + sizeBetweenText ;

	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, menu, NULL, NULL);
	SDL_RenderPresent(renderer);

	//free(text_surface);
	//free(text_texture);

	int choiceMenu = 0;
	ReturnCodeMenu menuState = ReturnCodeMenu::INIT;
	bool quit_menu = false;
	SDL_Event event;
	while (!quit_menu && SDL_WaitEvent(&event)){
		switch (event.type)
		{
		case SDL_QUIT:
			quit_menu = true;
			menuState = ReturnCodeMenu::QUIT_GAME;
			break;

		case SDL_KEYDOWN:

			switch( event.key.keysym.sym ){

			case SDLK_DOWN:
				choiceMenu+=1;
				choiceMenu = choiceMenu % numberChoice ;

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				cadre.y = sizeTetris.h/2 - (numberChoice * sizeBetweenText)/2
						+ (choiceMenu+1) * sizeBetweenText ;
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);

				break;

			case SDLK_UP:
				choiceMenu-=1 - numberChoice;
				choiceMenu = choiceMenu % numberChoice ;

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				cadre.y = sizeTetris.h/2 - (numberChoice * sizeBetweenText)/2
						+ (choiceMenu+1) * sizeBetweenText ;
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);
				break;

			case SDLK_RETURN:

				//quitter
				if (choiceMenu == 2){
					quit_menu = true;
					menuState = ReturnCodeMenu::QUIT_GAME;
				}
				//qaller au menu
				if (choiceMenu == 1){
					quit_menu = true;
					menuState = ReturnCodeMenu::GO_TO_MAIN_MENU;
				}

				//recommencer
				if (choiceMenu == 0){
					SDL_RenderClear(renderer);
					this->init(music, multiplayer);
					SDL_RenderPresent(renderer);
					menuState = this->loop(music, multiplayer);
					quit_menu = true;
				}
				break;

			case SDLK_ESCAPE:
				quit_menu = true;
				menuState = ReturnCodeMenu::QUIT_GAME;
				break;

			default:
				break;
			}

		default :break;
		}
	}

	return menuState;
}

ReturnCodeMenu Tetris::printMenu(){
	ReturnCodeMenu menuState = ReturnCodeMenu::INIT;
	int numberChoice = 4;
	int sizeBetweenText = 80, xShift = 100;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//on copie la texture de fond sur le menu
	SDL_SetRenderTarget(renderer, menu);
	SDL_SetRenderDrawColor(renderer,63,63,63,200);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderFillRect(renderer, NULL);

	printGenericMenu(menu, xShift, sizeBetweenText ,false, numberChoice, "Tetris", "Reprendre le jeu",
			"Recommencer", "Aller au menu", "Quitter");

	SDL_Rect cadre ={w/2-xShift-25,0, 250, 40};
	cadre.y = sizeTetris.h/2 - (numberChoice * sizeBetweenText)/2 + sizeBetweenText ;

	//on revient sur le renderer
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, menu, NULL, NULL);
	SDL_RenderPresent(renderer);

	//free(text_surface);
	//free(text_texture);

	int choiceMenu = 0;
	bool quit_menu = false;
	SDL_Event event;
	while (!quit_menu && SDL_WaitEvent(&event)){
		switch (event.type)
		{
		case SDL_QUIT:
			quit_menu = true;
			menuState = ReturnCodeMenu::QUIT_GAME;
			break;

		case SDL_KEYDOWN:

			switch( event.key.keysym.sym ){

			case SDLK_DOWN:
				choiceMenu+=1;
				choiceMenu = choiceMenu % numberChoice ;

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				cadre.y = sizeTetris.h/2 - (numberChoice * sizeBetweenText)/2
						+ (choiceMenu+1) * sizeBetweenText ;
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);

				break;

			case SDLK_UP:
				choiceMenu-=1 - numberChoice;
				choiceMenu = choiceMenu % numberChoice ;

				SDL_SetRenderDrawColor(renderer,0,0,0,255);
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				cadre.y = sizeTetris.h/2 - (numberChoice * sizeBetweenText)/2
						+ (choiceMenu+1) * sizeBetweenText ;
				SDL_RenderDrawRect(renderer, &cadre);

				SDL_RenderPresent(renderer);
				break;

			case SDLK_RETURN:

				//reprendre le jeu
				if (choiceMenu == 0)
					quit_menu = true;

				//recommencer
				if (choiceMenu == 1){
					quit_menu = true;
					menuState = ReturnCodeMenu::RESTART;
				}
				//go to menu (2)
				else if (choiceMenu == 3 || choiceMenu == 2){
					quit_menu = true;
					menuState = ReturnCodeMenu::GO_TO_MAIN_MENU;
				}

				//QUIT gAME
				if (choiceMenu == 3){
					quit_menu = true;
					menuState = ReturnCodeMenu::QUIT_GAME;
				 }
				break;

			case SDLK_ESCAPE:
				quit_menu = true;
				//quit = false;
				break;

			default:
				break;
			}


		default :break;
		}
	}

	return menuState;

}
