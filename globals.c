#include <SDL/SDL_image.h>
#include "globals.h"

KeysDown KeyDown;
TextColors TextColor;
ApplicationProperties Application;
MainMenuProperties MainMenu;
Images Image;
MatchProperties Match;
PROGRAM_STATE *STATE_MAIN_MENU,
			  *STATE_VIEW_HIGHSCORES,
			  *STATE_PLAYING,
			  *STATE_VICTORY,
			  *STATE_DEFEAT,
			  *STATE_QUITTING;


void PROGRAM_SWITCH_STATE(PROGRAM_STATE *State)
{
	Application.NextState = State;
}

int randmax(int be)
{
	return (int)((double)rand() / RAND_MAX * be);
}

SDL_Surface *IMG_Load_Optimized(char *filename)
{
	SDL_Surface *raw = IMG_Load(filename);
	SDL_Surface *optimized = SDL_DisplayFormat(raw);
	SDL_FreeSurface(raw);

	return optimized;
}
