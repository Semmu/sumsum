#include "globals.h"
#include "state_view_highscores.h"

void STATE_VIEW_HIGHSCORES_INIT()
{
	/*
	**	HÁTTÉR SÖTÉTÍTÉSE
	*/

	SDL_Surface *darken = SDL_CreateRGBSurface(SDL_SWSURFACE, Application.Output->w, Application.Output->h, Application.Output->format->BitsPerPixel, Application.Output->format->Rmask, Application.Output->format->Gmask, Application.Output->format->Bmask, Application.Output->format->Amask);
	SDL_FillRect(darken, NULL, SDL_MapRGB(darken->format, 0, 0, 0));
	SDL_SetAlpha(darken, SDL_SRCALPHA, 220);
	SDL_BlitSurface(darken, NULL, Application.Output, NULL);
	SDL_FreeSurface(darken);



	/*
	**	"HIGHSCORES"
	*/

	SDL_Surface *text = TTF_RenderText_Blended(Application.FONT_MORPHEUS_BIG, "HIGHSCORES", TextColor.WHITE);
	SDL_Rect r;
	r.x = Application.Output->w / 2 - text->w / 2;
	r.y = 50;
	SDL_BlitSurface(text, NULL, Application.Output, &r);
	SDL_FreeSurface(text);



	/*
	**	"< PRESS ESC TO GO BACK >"
	*/

	text = TTF_RenderText_Solid(Application.FONT, "< PRESS ESC TO GO BACK >", TextColor.LIGHT_GRAY);
	r.x = Application.Output->w / 2 - text->w / 2;
	r.y = Application.Output->h - 30;
	SDL_BlitSurface(text, NULL, Application.Output, &r);
	SDL_FreeSurface(text);



	/*
	**	HIGHSCORE-OK KIÍRÁSA
	*/

	Highscore scores[10];
	int c;

	FILE *in = fopen("highscores.dat", "rt");
	for (c = 0; c < 10; c++)
		fscanf(in, "%[^\n,],%d\n", scores[c].name, &scores[c].score);
	fclose(in);

	char scoretxt[10]; // pontszám sztringje
	r.y = 200;
	for (c = 0; c < 10; c++)
	{
		/*
		**	"JÁTÉKOS NEVE"
		*/

		text = TTF_RenderText_Blended(Application.FONT_MORPHEUS, scores[c].name, TextColor.GOLD);
		r.x = Application.Output->w / 2 - HIGHSCORES_WIDTH / 2;
		SDL_BlitSurface(text, NULL, Application.Output, &r);
		SDL_FreeSurface(text);



		/*
		**	JÁTÉKOS "PONTSZÁMA"
		*/

		sprintf(scoretxt, "%d", scores[c].score);
		text = TTF_RenderText_Blended(Application.FONT_MORPHEUS, scoretxt, TextColor.GOLD);
		r.x = Application.Output->w / 2 + HIGHSCORES_WIDTH / 2 - text->w;
		SDL_BlitSurface(text, NULL, Application.Output, &r);
		SDL_FreeSurface(text);

		r.y += 40; // következő sor lejjebb rakása
	}
}

void STATE_VIEW_HIGHSCORES_UNINIT()
{
	// nincs mit feltakarítani

	return;
}

void STATE_VIEW_HIGHSCORES_LOOP()
{
	SDL_WaitEvent(&Application.e);

	if (Application.e.type == SDL_KEYDOWN && Application.e.key.keysym.sym == SDLK_ESCAPE)
	{
		PROGRAM_SWITCH_STATE(STATE_MAIN_MENU);
	}
}

void STATE_VIEW_HIGHSCORES_RESIZE()
{
	// Átméretezéskor újra kell inicializálni a highscore állapotot.
	STATE_VIEW_HIGHSCORES_INIT();
}
