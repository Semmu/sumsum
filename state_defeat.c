#include "globals.h"
#include "state_defeat.h"

void STATE_DEFEAT_INIT()
{
	/*
	**	HÁTTÉR ELSÖTÉTÍTÉSE
	*/

	SDL_Surface *darken = SDL_CreateRGBSurface(SDL_SWSURFACE, Application.Output->w, Application.Output->h, Application.Output->format->BitsPerPixel, Application.Output->format->Rmask, Application.Output->format->Gmask, Application.Output->format->Bmask, Application.Output->format->Amask);
	SDL_FillRect(darken, NULL, SDL_MapRGB(darken->format, 0, 0, 0));
	SDL_SetAlpha(darken, SDL_SRCALPHA, 220);
	SDL_BlitSurface(darken, NULL, Application.Output, NULL);
	SDL_FreeSurface(darken);



	/*
	**	"YOU HAVE BEEN DEFEATED!"
	*/

	SDL_Surface *text = TTF_RenderText_Blended(Application.FONT_MORPHEUS_BIG, "YOU HAVE BEEN DEFEATED!", TextColor.RED);
	SDL_Rect r;
	r.x = Application.Output->w / 2 - text->w / 2;
	r.y = Application.Output->h / 2 - text->h / 2;
	SDL_BlitSurface(text, NULL, Application.Output, &r);
	SDL_FreeSurface(text);



	/*
	**	"< PRESS ENTER TO RETURN TO MENU >"
	*/

	text = TTF_RenderText_Solid(Application.FONT, "< PRESS ENTER TO RETURN TO MENU >", TextColor.LIGHT_GRAY);
	r.x = Application.Output->w / 2 - text->w / 2;
	r.y = Application.Output->h - 30;
	SDL_BlitSurface(text, NULL, Application.Output, &r);
	SDL_FreeSurface(text);
}

void STATE_DEFEAT_LOOP()
{
	SDL_WaitEvent(&Application.e);

	if (Application.e.type == SDL_KEYDOWN && Application.e.key.keysym.sym == SDLK_RETURN)
	{
		PROGRAM_SWITCH_STATE(STATE_MAIN_MENU);
	}
}

void STATE_DEFEAT_UNINIT()
{
	// nincs mit feltakarítani

	return;
}
