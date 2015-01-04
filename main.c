#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "globals.h"
#include "state_main_menu.h"
#include "state_playing.h"
#include "state_victory.h"
#include "state_defeat.h"
#include "state_view_highscores.h"

void empty_funtcion() {}

int main()
{
	/*
	**	FÜGGVÉNYKÖNYVTÁRAK INICIALIZÁLÁSA
	*/

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	srand(time(NULL));




	/*
	**	A Application FŐBB RÉSZEINEK BEÁLLÍTÁSA
	*/



	Application.LPS_CAP = 50;
	//Application.Output = SDL_SetVideoMode(0, 0, 0, SDL_ANYFORMAT | SDL_FULLSCREEN);
	Application.Output = SDL_SetVideoMode(1300, 700, 0, SDL_ANYFORMAT);
	SDL_WM_SetCaption("S.U.M.S.U.M.",  "S.U.M.S.U.M.");
	SDL_ShowCursor(SDL_DISABLE);
	Application.FONT = TTF_OpenFont("res/font", 0);
	Application.FONT_MORPHEUS = TTF_OpenFont("res/morpheus.ttf", 30);
	Application.FONT_MORPHEUS_BIG = TTF_OpenFont("res/morpheus.ttf", 90);


	STATE_MAIN_MENU = (PROGRAM_STATE*)malloc(sizeof(PROGRAM_STATE));
	STATE_MAIN_MENU->INIT = &STATE_MAIN_MENU_INIT;
	STATE_MAIN_MENU->LOOP = &STATE_MAIN_MENU_LOOP;
	STATE_MAIN_MENU->UNINIT = &STATE_MAIN_MENU_UNINIT;

	STATE_VIEW_HIGHSCORES = (PROGRAM_STATE*)malloc(sizeof(PROGRAM_STATE));
	STATE_VIEW_HIGHSCORES->INIT = &STATE_VIEW_HIGHSCORES_INIT;
	STATE_VIEW_HIGHSCORES->LOOP = &STATE_VIEW_HIGHSCORES_LOOP;
	STATE_VIEW_HIGHSCORES->UNINIT = &STATE_VIEW_HIGHSCORES_UNINIT;

	STATE_PLAYING = (PROGRAM_STATE*)malloc(sizeof(PROGRAM_STATE));
	STATE_PLAYING->INIT = &STATE_PLAYING_INIT;
	STATE_PLAYING->LOOP = &STATE_PLAYING_LOOP;
	STATE_PLAYING->UNINIT = &STATE_PLAYING_UNINIT;

	STATE_VICTORY = (PROGRAM_STATE*)malloc(sizeof(PROGRAM_STATE));
	STATE_VICTORY->INIT = &STATE_VICTORY_INIT;
	STATE_VICTORY->LOOP = &STATE_VICTORY_LOOP;
	STATE_VICTORY->UNINIT = &STATE_VICTORY_UNINIT;

	STATE_DEFEAT = (PROGRAM_STATE*)malloc(sizeof(PROGRAM_STATE));
	STATE_DEFEAT->INIT = &STATE_DEFEAT_INIT;
	STATE_DEFEAT->LOOP = &STATE_DEFEAT_LOOP;
	STATE_DEFEAT->UNINIT = &STATE_DEFEAT_UNINIT;

	STATE_QUITTING = (PROGRAM_STATE*)malloc(sizeof(PROGRAM_STATE));
	STATE_QUITTING->INIT = &empty_funtcion;
	STATE_QUITTING->LOOP = &empty_funtcion;
	STATE_QUITTING->UNINIT = &empty_funtcion;


	Application.State = STATE_MAIN_MENU;
	Application.NextState = STATE_MAIN_MENU;


	/*
	**	SZÍNÉRTÉKEK FELTÖLTÉSE
	*/

	ColorValue.INT[RED] = SDL_MapRGB(Application.Output->format, 255, 0, 0);
	ColorValue.INT[YELLOW] = SDL_MapRGB(Application.Output->format, 255, 255, 0);
	ColorValue.INT[GREEN] = SDL_MapRGB(Application.Output->format, 0, 255, 0);
	ColorValue.INT[BLUE] = SDL_MapRGB(Application.Output->format, 0, 0, 255);
	ColorValue.INT[PURPLE] = SDL_MapRGB(Application.Output->format, 200, 0, 200);
	ColorValue.INT[PINK] = SDL_MapRGB(Application.Output->format, 255, 100, 255);
	ColorValue.INT[BLACK] = SDL_MapRGB(Application.Output->format, 0, 0, 0);
	ColorValue.INT[WHITE] = SDL_MapRGB(Application.Output->format, 255, 255, 255);




	/*
	**	SZÖVEGSZÍNEK FELTÖLTÉSE
	*/

	TextColor.WHITE.r = TextColor.WHITE.g = TextColor.WHITE.b = 255;
	TextColor.BLACK.r = TextColor.BLACK.g = TextColor.BLACK.b = 0;
	TextColor.LIGHT_GRAY.r = TextColor.LIGHT_GRAY.g = TextColor.LIGHT_GRAY.b = 150;
	TextColor.GOLD.r = 230;
	TextColor.GOLD.g = 160;
	TextColor.GOLD.b = 30;

	TextColor.RED.r = 200;
	TextColor.RED.g = 10;
	TextColor.RED.b = 10;




	/*
	**	KÉPEK BETÖLTÉSE
	*/

	Image.MAIN_MENU_BG = IMG_Load_Optimized("res/menu_bg.png");

	Image.Town[RED] =		IMG_Load("res/town_red.png");
	Image.Town[YELLOW] =	IMG_Load("res/town_yellow.png");
	Image.Town[GREEN] =	IMG_Load("res/town_green.png");
	Image.Town[BLUE] =	IMG_Load("res/town_blue.png");
	Image.Town[PURPLE] =	IMG_Load("res/town_purple.png");
	Image.Town[PINK] =	IMG_Load("res/town_pink.png");
	Image.Town[BLACK] =	IMG_Load("res/town_black.png");
	Image.Town[WHITE] =	IMG_Load("res/town_white.png");

	Image.Unit[RED] =		IMG_Load("res/unit_red.png");
	Image.Unit[YELLOW] =	IMG_Load("res/unit_yellow.png");
	Image.Unit[GREEN] =	IMG_Load("res/unit_green.png");
	Image.Unit[BLUE] =	IMG_Load("res/unit_blue.png");
	Image.Unit[PURPLE] =	IMG_Load("res/unit_purple.png");
	Image.Unit[PINK] =	IMG_Load("res/unit_pink.png");
	Image.Unit[BLACK] =	IMG_Load("res/unit_black.png");
	Image.Unit[WHITE] =	IMG_Load("res/unit_white.png");

	Image.GRASS = IMG_Load_Optimized("res/grass.png");

	Image.SELECTED_TOWN_BORDER = IMG_Load("res/selected_town_border.png");
	Image.SELECTED_ENEMY_TOWN_BORDER = IMG_Load("res/selected_enemy_town_border.png");

	Image.HAND = IMG_Load("res/hand.png");

	IMG_Quit(); // minden képet betöltöttem




	/*
	**	FŐMENÜ BEÁLLÍTÁSA
	*/

	strcpy(MainMenu.PlayerName, "Player");
	MainMenu.NumberOfEnemies = 2;
	MainMenu.Difficulty = DIFFICULTY_NORMAL;
	MainMenu.StartingResources = RESOURCES_NORMAL;
	MainMenu.MapSize = MAP_SIZE_NORMAL;
 	MainMenu.State = STATE_MAIN_MENU_PLAYER_COLOR;




	/*
	**	Application FUTTATÁSA
	*/

	while (Application.State != STATE_QUITTING)
	{
		Application.LastRun = SDL_GetTicks();

		Application.State->LOOP();


		if (Application.NextState != NULL && Application.NextState != Application.State) // ha állapotváltás van beütemezve
		{
			Application.NextState->INIT();

			Application.State->UNINIT();

			Application.State = Application.NextState; // új állapot beállítása
			Application.NextState = NULL;
		}


		/*
		**	Application "PIHENTETÉSE"
		*/

		SDL_Flip(Application.Output);
		if (Application.State == STATE_PLAYING && 1000 / Application.LPS_CAP > SDL_GetTicks() - Application.LastRun)
			SDL_Delay(1000 / Application.LPS_CAP - SDL_GetTicks() + Application.LastRun);
	}

	Application.State->UNINIT();

	SDL_Quit();

	return 0;
}
