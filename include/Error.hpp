/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef _ERROR_HPP_
#define _ERROR_HPP_

class Error
{
public:
	
	//Error();
	
	bool ERROR;
	bool OVER_X;
	bool OVER_Y;
	bool COLLISION_PIECE;
	bool NO_ERROR;
	
	int OVER_RIGHT_X;
	int OVER_LEFT_X;
	int OVER_UP_y;
	int OVER_DOWN_y;
	int OVER_NUMBER_X;
	
	Error(){
		ERROR = false;
		COLLISION_PIECE = false;
		OVER_X = false;
		OVER_Y = false;
		NO_ERROR = true;
		OVER_RIGHT_X = 0;
		OVER_LEFT_X = 0;
		OVER_UP_y = 0;
		OVER_DOWN_y = 0;
		OVER_NUMBER_X = 0;
	}
};



#endif

#ifndef _RETURN_CODE_HPP_
#define _RETURN_CODE_HPP_

/**
 * This enum class is useful to navigate between menu. For example, if the 
 * pause menu is called, it can returned RESTART to the calling function.
 */
enum class ReturnCodeMenu { 
	QUIT_MENU,
	QUIT_GAME,
	RESTART,
	GO_TO_MAIN_MENU,
	INIT,
	GAME_OVER
};
#endif