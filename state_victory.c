#include "globals.h"
#include "state_victory.h"
#include <math.h>

void STATE_VICTORY_INIT()
{
	/*
	**	SCORE KISZÁMÍTÁS
	*/

	int score = (1000.0 - sqrt((double)(SDL_GetTicks() - Match.STARTED))) * Match.NumberOfEnemies * MainMenu.Difficulty * MainMenu.MapSize * MainMenu.StartingResources / 1000;
	char scoretxt[20]; // score -> string
	sprintf(scoretxt, "SCORE: %d", score);



	/*
	**	HIGHSCORES MEGNYITÁS ÉS KIÍRÁS HA KELL
	*/

	Highscore scores[10];
	int c, i;

	FILE *in = fopen("highscores.dat", "rt");
	for (c = 0; c < 10; c++)
		fscanf(in, "%[^\n,],%d\n", scores[c].name, &scores[c].score);
	fclose(in);

	c = 0;
	while (scores[c].score > score) c++;

	if (c < 10)
	{
		/*
		**	HA BE KELL ÍRNI A HIGHSCORES-BA
		*/

		for (i = 9; i > c; i--)
		{
			// az utánalevőket egyesével áthelyezem

			scores[i].score = scores[i-1].score;
			strcpy(scores[i].name, scores[i-1].name);
		}

		// beírom az aktuális meccs végeredményét

		scores[c].score = score;
		strcpy(scores[c].name, MainMenu.PlayerName);

		/*
		**	MEGVÁLTOZOTT HIGHSCORES KIÍRÁSA
		*/

		in = fopen("highscores.dat", "wt");
		for (c = 0; c < 10; c++)
			fprintf(in, "%s,%d\n", scores[c].name, scores[c].score);
		fclose(in);
	}





	/*
	**	KÉP SÖTÉTÍTÉSE
	*/

	SDL_Surface *darken = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, Application.Output->w, Application.Output->h, Application.Output->format->BitsPerPixel, Application.Output->format->Rmask, Application.Output->format->Gmask, Application.Output->format->Bmask, Application.Output->format->Amask);
	SDL_SetAlpha(darken, SDL_SRCALPHA, 220);
	SDL_FillRect(darken, NULL, SDL_MapRGB(darken->format, 0, 0, 0));
	SDL_BlitSurface(darken, NULL, Application.Output, NULL);
	SDL_FreeSurface(darken);


	/*
	**	"VICTORY!"
	*/

	SDL_Surface *text = TTF_RenderText_Blended(Application.FONT_MORPHEUS_BIG, "VICTORY!", TextColor.GOLD);
	SDL_Rect r;
	r.x = Application.Output->w / 2 - text->w / 2;
	r.y = Application.Output->h / 2 - text->h / 2;
	SDL_BlitSurface(text, NULL, Application.Output, &r);
	SDL_FreeSurface(text);


	/*
	** SCORE KIÍRÁSA
	*/

	text = TTF_RenderText_Blended(Application.FONT_MORPHEUS, scoretxt, TextColor.WHITE);
	r.x = Application.Output->w / 2 - text->w / 2;
	r.y = Application.Output->h / 2 - text->h / 2 + 60;
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

void STATE_VICTORY_LOOP()
{
	SDL_WaitEvent(&Application.e);

	if (Application.e.type == SDL_KEYDOWN && Application.e.key.keysym.sym == SDLK_RETURN)
	{
		PROGRAM_SWITCH_STATE(STATE_MAIN_MENU);
	}
}

void STATE_VICTORY_UNINIT()
{
	// nincs mit feltakarítani

	return;
}
