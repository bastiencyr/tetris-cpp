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
#include "../include/tetris.hpp"
#include <SDL2/SDL_ttf.h>
#include "../include/Error.hpp"
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
//	pWindow = SDL_CreateWindow("Une fenetre SDL" , SDL_WINDOWPOS_CENTERED ,
//			SDL_WINDOWPOS_CENTERED , w , h , SDL_WINDOW_SHOWN);
	//renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	blank = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	menu = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);

	//mat[BLOCSX][BLOCSY];
	for(auto &raw : mat){
		for(auto &el : raw)
			el = false;
	}

	for(auto &raw : matIA){
		for(auto &el : raw)
			el = false;
	}

	for(int j = 0; j< BLOCSY; j++) {
		for(int i = 0; i< BLOCSX; i++) {
			if (mat[i][j]==true)
				std::cout << "--0--" ;
			else
				std::cout << "--1--" ;
		}
		std::cout << std::endl;
	}

	sizeTetris.w = locTetris.w;
	sizeTetris.h = sizeTetris.w*2;
	sizeTetris.x = locTetris.x;
	sizeTetris.y = locTetris.y;

	int sizeCase = locTetris.w/BLOCSX;

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

	for(auto &raw : mat){
		for(auto &el : raw)
			el = false;
	}

	for(auto &raw : matIA){
		for(auto &el : raw)
			el = false;
	}

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


	//affichage du texte
	TTF_Font *police = TTF_OpenFont("src/Tetris.ttf", 65);
	if(!police){
		std::cout << TTF_GetError()<< std::endl;
	}
	SDL_Color textColor = {255, 255, 255};

	SDL_Surface * text_surface = TTF_RenderText_Blended(police,"Next Piece", textColor);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	int sCase = sizeTetris.w/BLOCSX;
	int texW = sCase * 3;
	int texX = sizeTetris.w + 2*sCase + sizeTetris.x;
	int texY = sizeTetris.h/2 - sCase * 3 + sizeTetris.y;
	SDL_Rect dstrect = { texX, texY, texW, 25 };

	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);

	FREE_TEXTURE(text_texture);
	FREE_SURFACE(text_surface);
	TTF_CloseFont(police);

}

//MIGRATION OK REMPLACER DANS LE CONSTRUCTEUR DE PLAYER
void Tetris::ListePieceInit(Piece * Liste[7]) {
	Liste[0] = new LTetri(options);
	Liste[1] = new OTetri(options);
	Liste[2] = new TTetri(options);
	Liste[3] = new ZTetri(options);
	Liste[4] = new JTetri(options);
	Liste[5] = new ITetri(options);
	Liste[6] = new STetri(options);

	for(int i = 0; i<7; i++)
		Liste[i]->update();
}

//REMPLACE PAR nouvPiece(Piece * & oldp, Piece *& newp)
//DRAW nEST PAS DEPLACE ET DOIT DONC ETRE AJOUTER EN DEHORS DE LA FONCTION
ReturnCodeMenu Tetris::NouvPiece(Piece * & oldp, Piece *& newp, Piece * Liste[7]) {
	ReturnCodeMenu gameState = ReturnCodeMenu::INIT ;
	oldp = newp;
	oldp->update();

	int randn = rand() % 7;
	newp = Liste[randn];
	newp->update();
	newp->printNextPiece2(renderer, blank, texture);

	if(!oldp->isLegalPosition(oldp, mat).NO_ERROR) {
		gameState = ReturnCodeMenu::GAME_OVER ;
		std::cout<< "Game Over" << std::endl << "Score : " << score << std::endl;
	}
	else
		oldp->draw(renderer,blank,texture);

	return gameState;
}

ReturnCodeMenu Tetris::loop(Mix_Music* music, bool multiplayer){

	auto ghostVerifDraw = [&] (Piece *ghost, Piece *piece, bool gen =false)
	{
		ghost->DownGhost(mat,piece,gen);
		ghost->verif(piece);
		ghost->draw(renderer,blank,texture,OPAC);
	};

	Piece::initStaticMembers(sizeTetris);

	Mix_Music* rotate = Mix_LoadMUS("sfx/SFX_PieceRotateLR.ogg");
	Mix_Music* drop = Mix_LoadMUS("sfx/SFX_PieceSoftDrop.ogg");
	Mix_Music* line = Mix_LoadMUS("sfx/SFX_SpecialLineClearSingle.ogg");
	Mix_Music* lines = Mix_LoadMUS("sfx/SFX_SpecialLineClearTriple.ogg");

	//INITIALISATION
	if (Mix_PlayingMusic() == 0) Mix_PlayMusic(music,-1);
	Uint64 prev, now = SDL_GetPerformanceCounter(); // timers
	double difficulte[18]={1.,0.9,0.82,0.72,0.61,0.52,0.45,0.4,0.37,0.35,0.34,0.33,0.32,0.31,0.3,0.29,0.28,0.27}; //difficulté
	double difficulteIA[18]={1.,0.9,0.82,0.72,0.61,0.52,0.45,0.4,0.37,0.35,0.34,0.33,0.32,0.31,0.3,0.29,0.28,0.27}; //difficulté
	int sc=0; //niveau de difficulté actuel
	int scIA = 0;

	bool quit_loop = false;
	ReturnCodeMenu gameState = ReturnCodeMenu::INIT ;
	score = 0 ;
	scoreIA = 0;
	bool cont = true, win = true;
	double t=0, delta_t=0, tIA =0, delta_tIA = 0;
	int scoreOldIA = score+1, ScoreOld=score-1;
	int d=0, dia = 0;

	if (multiplayer){
		this->printScore(scoreIA, 1.6 * sizeTetris.w, sizeTetris.h/7);
		this->printScore(score, 1.1 * sizeTetris.w, sizeTetris.h/7);
	}
	else
		this->printScore(score, 0.6 * sizeTetris.w, sizeTetris.h/7);


	Piece * PiecList[7], * PiecListIA[7], *PiecGhosts[7];
	ListePieceInit(PiecList);
	ListePieceInit(PiecListIA);
	ListePieceInit(PiecGhosts);

	srand(time(0));
	int randn= rand() % 7;

	Piece *piece, *newPiece, *pieceIA, *ghost, *reserve, *temp;
	reserve = nullptr; temp = nullptr;
	newPiece = PiecList[randn];
	piece = PiecList[randn];
	NouvPiece(piece, newPiece, PiecList) ;

	for(int i = 0; i<7; i++)
		PiecGhosts[i]->adjust(PiecList[i]);

	ghost=PiecGhosts[randn];
	ghost->adjust(piece);
	ghost->DownGhost(mat,piece,1);
	ghost->draw(renderer,blank,texture,OPAC);

	int sizeBloc = sizeTetris.w/BLOCSX;
	//BOUCLE
	while (!quit_loop)
	{
		if (Mix_PlayingMusic() == 0) Mix_PlayMusic(music,-1);

		if(!cont) {
			gameState = NouvPiece(piece, newPiece, PiecList);
			if(gameState == ReturnCodeMenu::GAME_OVER){
				win = false;
				quit_loop = true;
			}

			cont = true;
			/*
			//Mix_PlayMusic(drop, 0);
			*/
			ghost->adjust(piece);
			ghostVerifDraw(ghost, piece, true);
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
					piece->holdPiece(mat);
					piece->draw(renderer,blank,texture);
					cont = false;
					break;

				case SDLK_F1:
					piece->cheat(mat);
					for(int i = 0; i < 4; i++)
						mat[piece->getx(i)][piece->gety(i)]=true;
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
					}
					piece->update();
					ghost->adjust(piece);
					ghostVerifDraw(ghost, piece, true);
					reserve->update();
					reserve->printreserve(renderer, blank, texture, multiplayer);
					break;

				case SDLK_ESCAPE:
					gameState = this->printMenu();
					if (gameState == ReturnCodeMenu::QUIT_GAME
							or gameState == ReturnCodeMenu::GO_TO_MAIN_MENU)
						quit_loop = true;
					break;

				case SDLK_RIGHT:
					if (piece->isLegalRight(mat).NO_ERROR){
						piece->right();
						piece->draw(renderer,blank,texture);
						//Mix_PlayMusic(rotate, 0);

						ghostVerifDraw(ghost, piece);
					}
					break;

				case SDLK_LEFT:
					if (piece->isLegalLeft(mat).NO_ERROR){
						piece->left();
						piece->draw(renderer,blank,texture);
						//Mix_PlayMusic(rotate, 0);

						ghostVerifDraw(ghost, piece);
					}
					break;

				case SDLK_DOWN:
					cont = piece->onDown(mat, cont, renderer, blank,
							texture);
					break;

				case SDLK_UP:
					if (piece->isLegalRotateRight(mat).NO_ERROR){
						piece->rotateRight();
						piece->draw(renderer,blank,texture);

					}
					else if (piece->isLegalRotateRight(mat).OVER_X){
						Piece temp = *piece;
						int shift = -piece->isLegalRotateRight(mat).OVER_NUMBER_X;
						temp.rotateRight();
						if (temp.isLegalTranslate(shift, 0, mat).NO_ERROR){
							piece->rotateRight();
							piece->translate(shift, 0, false);
							piece->draw(renderer,blank,texture);
						}

					}
					ghostVerifDraw(ghost, piece);
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
		if(t>=difficulte[sc]) {
			cont = piece->onDown(mat, cont, renderer,
					blank,texture);
			t=0;
		}

		//nouvPiece for IA
		if (multiplayer and tIA>=2 ){
			randn = rand() % 7;
			pieceIA = PiecListIA[randn];
			pieceIA->update();
			pieceIA->cheat(matIA);

			if (!pieceIA->isLegalPosition(pieceIA, matIA).NO_ERROR){
				quit_loop = true;
				gameState = ReturnCodeMenu::GAME_OVER;
				pieceIA->affiche_coord(true, true);
			}
			for (int i=0; i<4; i++){
				matIA[pieceIA->getx(i)][pieceIA->gety(i)]=true;
			}
			pieceIA->draw(renderer,blank,texture, 255, false,
					sizeTetris2.x/sizeBloc);

			tIA = 0;
		}

		d=TetrisLinesUpdate(&score);
		//if(d==1) Mix_PlayMusic(line, 0);
		//else if(d>1) Mix_PlayMusic(lines, 0);

		//update score
		if (multiplayer){
			dia = TetrisLinesUpdate(&scoreIA, true);
			if(dia>=1){
				this->addLineToPlayer(dia-1, piece, ghost);
				this->updateLevel(scoreIA, scoreOldIA, scIA);
				printScore(scoreIA, 1.6 * sizeTetris.w, sizeTetris.h/7);
			}

			if (d >= 1){
				this->updateLevel(score, ScoreOld, sc);
				this->addLineToPlayer(d-1, pieceIA, ghost, true);
				printScore(score, 1.1 * sizeTetris.w, sizeTetris.h/7);
			}
		}
		if (!multiplayer and d>0)
			printScore(score, 0.6 * sizeTetris.w, sizeTetris.h/7);

		SDL_RenderPresent(renderer);
	}
	for (int i = 0; i < 7; i++){
		delete PiecGhosts[i];
		delete PiecList[i];
		delete PiecListIA[i];
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

//REMPLACER PAR updateLevel(int& ScoreOld);
void Tetris::updateLevel(int& score, int& ScoreOld, int& sc){
	if (score-ScoreOld>500) {
		ScoreOld=score;
		if(sc!=17) {
			sc++;
			std::cout << "Niveau supérieur !" << std::endl;
			std::cout << "		Niveau :" << sc << std::endl;
		}
	}
}

void Tetris::printScore(int& score, int xScore, int yScore){

	TTF_Font * police = TTF_OpenFont("src/RetroGaming.ttf", 65);
	if(!police){
		std::cout << TTF_GetError()<< std::endl;
	}

	//affchage de "score"
	SDL_Color textColor = {255, 255, 255};
	SDL_Surface * text_surface = TTF_RenderText_Blended(police, "Score", textColor);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	int scoreW = text_surface->w/2, scoreH = text_surface->h/2;
	SDL_Rect dstrect = {0,0, scoreW, scoreH };

	dstrect.x = xScore;
	dstrect.y = yScore;

	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, &dstrect, &dstrect);

	std::string scoreStr = std::to_string ( score );
	char * scoreStrArr = &scoreStr[0];

	FREE_SURFACE(text_surface);
	FREE_TEXTURE(text_texture);

	//print score
	text_surface = TTF_RenderText_Solid(police, scoreStrArr, textColor);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect dstrect2 = {0,0, text_surface->w/2, text_surface->h/2 };
	dstrect2.x = xScore + scoreW/2 - text_surface->w/4 ;
	dstrect2.y = yScore + 60;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderFillRect(renderer, &dstrect2);

	//on affiche une ligne de séparation
	//if (player2){
	//	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	//	SDL_RenderDrawLine(renderer,3*sizeTetris.w/2, 60,
	//			3*sizeTetris.w/2,250);
	//}

	SDL_RenderCopy(renderer, text_texture, NULL, &dstrect2);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, &dstrect2, &dstrect2);

	TTF_CloseFont(police);
	FREE_SURFACE(text_surface);
	FREE_TEXTURE(text_texture);
	police = nullptr;

}

void Tetris::addLineToPlayer(int nbLineToAdd, Piece *piece, Piece *ghost, bool player2){
	//player2 = true -> ajouter une ligne à l'IA
	for(int i=0 ; i< nbLineToAdd ;i++){
		SDL_SetRenderDrawColor(renderer,100,100,100,255);
		srand(time(0));
		int randn= rand() % 10;

		int factor = sizeTetris.w/BLOCSX;
		//ajouter une ligne à player1
		for (int i = 0; i< BLOCSY ; i++)
			CopyLine(i, -1, 0, player2);

		if (player2){
			for(int j=0 ; j< BLOCSX ; j++)
				matIA[j][BLOCSY-1] = true;
			matIA[randn][BLOCSY-1] = false;
		}
		else {
			for(int j=0 ; j< BLOCSX ; j++)
				mat[j][BLOCSY-1] = true;
			mat[randn][BLOCSY-1] = false;
		}

		for(int j = 0; j< BLOCSX ; j++){
			SDL_SetRenderTarget(renderer, texture);

			SDL_Rect line = {
				sizeTetris.x + j*factor,
				(BLOCSY-1)*factor+ sizeTetris.y,
				factor,
				factor,
			};
			if (player2) line.x = sizeTetris2.x + j*factor;
			SDL_RenderCopy(renderer, blank, &line, &line);

			if(j!=randn){
				line.x= sizeTetris.x + j*factor + 5;
				line.y= (BLOCSY-1)*factor+ sizeTetris.y + 5;
				line.h= factor - 10;
				line.w= factor - 10;
				if (player2) line.x= sizeTetris2.x + j*factor + 5;

				SDL_RenderFillRect(renderer, &line);
				SDL_SetRenderTarget(renderer, NULL);
				SDL_RenderCopy(renderer, texture, &line, &line);
			}
		}
		piece->up();

		//A laisser ?
		piece->mvDstToSrc(*piece);

		if(!player2){
			ghost->up();
			ghost->DownGhost(mat,piece);
			ghost->verif(piece);

			ghost->draw(renderer,blank,texture,OPAC);
			piece->draw(renderer,blank,texture);
		}
		SDL_RenderPresent(renderer);
	}
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
					score = 0 ;
					for(int i = 0; i < BLOCSX; i++) {
						for(int j = 0; j < BLOCSY; j++) {
							mat[i][j] = false;
						}
					}
					SDL_SetRenderTarget(renderer, texture);
					SDL_RenderCopy(renderer, blank, &sizeTetris, &sizeTetris);
					quit_menu = true;
					menuState = ReturnCodeMenu::RESTART;
					//quit = false;
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

int Tetris::TetrisLinesUpdate(int *score, bool player2) {

	int decalage = 0;
	for(int i = BLOCSY-1; i>=0; i--) {

		int compt = 0;
		for(int j = 0; j<BLOCSX; j++) {
			if(player2 and matIA[j][i]) compt++;
			if(!player2 and mat[j][i]) compt++;
		}
		if(compt==BLOCSX) //ligne pleine
		{
			decalage++;
			if (player2)
				FillEmpty(i,SIZE_BLOC, player2);
			else
				FillEmpty(i,SIZE_BLOC);
		}

		else if(compt < BLOCSX && compt != 0 && decalage!=0)
		{
			if (player2){
				CopyLine(i, decalage, SIZE_BLOC, player2);
				FillEmpty(i,SIZE_BLOC, player2);
			}
			else {
				CopyLine(i, decalage, SIZE_BLOC, player2);
				FillEmpty(i,SIZE_BLOC, player2);
			}
		}
		//else
		//	break;
	}
	switch(decalage) {
		case 1:
			*score+=100;
			break;
		case 2:
			*score+=300;
			break;
		case 3:
			*score+=500;
			break;
		case 4:
			*score+=800;
			break;
		default:
			break;
		}
	if(decalage!=0) {
		std::cout << "Bravo !" << std::endl;
		std::cout << "Score : " << *score << std::endl;
	}
	return decalage;
}

//void Player::FillEmpty(int i)
void Tetris::FillEmpty(int i,int factore, bool player2) {
	int factor = sizeTetris.w/BLOCSX;
	SDL_Rect line;
	line.x= 0 + sizeTetris.x;
	if (player2)
		line.x= 0 + sizeTetris2.x;

	line.y= i*factor+ sizeTetris.y;
	line.h= 1*factor;
	line.w= BLOCSX*factor;
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, blank, &line, &line);
	SDL_SetRenderTarget(renderer, NULL);

	if (player2){
		for (auto &row : matIA)
			row[i]=false;
	}
	else {
		for (auto &row : mat)
			row[i]=false;
	}
}

void Tetris::CopyLine(int i, int decalage, int factore, bool player2) {
	int factor = sizeTetris.w/BLOCSX;
	SDL_Texture* temp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, this->w, this->h);
	//(je sais pas si ça marche) On copie la texture originelle sur le truc temporaire (y a une vraie raison à ça)
	SDL_SetRenderTarget(renderer, temp);
	SDL_RenderCopy(renderer, blank, NULL, NULL);

	SDL_Rect copyline;
	SDL_Rect copytext;

	copyline.h= factor;
	copyline.w= factor*BLOCSX;
	copyline.y= (i+decalage)*factor+ sizeTetris.y;
	copyline.x= 0 + sizeTetris.x;
	if (player2)
		copyline.x= 0 + sizeTetris2.x;

	copytext.h= factor;
	copytext.w= factor;
	copytext.y= i*factor+ sizeTetris.y;
	copytext.x= 0 + sizeTetris.x;
	if (player2)
		copytext.x= 0 + sizeTetris2.x;

	for(int j = 0; j < BLOCSX; j++) {
		if(!player2 and mat[j][i]) {
			copytext.x=j*factor + sizeTetris.x;
			SDL_RenderCopy(renderer, texture, &copytext, &copytext);
			mat[j][i+decalage]=mat[j][i];
			mat[j][i]=0;
		}
		if(player2 and matIA[j][i]) {
			copytext.x=j*factor + sizeTetris2.x;
			SDL_RenderCopy(renderer, texture, &copytext, &copytext);
			matIA[j][i+decalage]=matIA[j][i];
			matIA[j][i]=0;
		}
	}

	SDL_SetRenderTarget(renderer, texture);

	copytext.w= factor*BLOCSX;
	copytext.x=0 + sizeTetris.x;
	if (player2)
		copytext.x=0 + sizeTetris2.x;

	SDL_RenderCopy(renderer, temp, &copytext, &copyline);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	FREE_TEXTURE(temp);

}

void Tetris::printMatrice(){
	std::cout << "L'état de la matrice est : " << std::endl;
	for(int j = 0; j< BLOCSY; j++) {
		for(int i = 0; i< BLOCSX; i++) {
			if (mat[i][j])
				std::cout << "--0--" ;
			else
				std::cout << "--1--" ;
		}
		std::cout << std::endl;
	}
}
