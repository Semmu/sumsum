#include <math.h>
#include "globals.h"
#include "state_playing.h"
#include "game_methods.h"

void STATE_PLAYING_INIT()
{
	KeyDown.Left = KeyDown.Right = KeyDown.Up = KeyDown.Down = 0;

	Match.NumberOfEnemies = MainMenu.NumberOfEnemies;
	Match.Difficulty = MainMenu.Difficulty;
	Match.MINIMUM_DISTANCE = MainMenu.MapSize;
	Match.PlayerColor = MainMenu.PlayerColor;
	strcpy(Match.PlayerName, MainMenu.PlayerName);

	Match.STARTED = SDL_GetTicks();

	Match.STARTING_PEASANTS = (MainMenu.StartingResources == RESOURCES_LESS ? 10 : (MainMenu.StartingResources == RESOURCES_NORMAL ? 20 : 40));
	Match.StartingResources = (MainMenu.StartingResources == RESOURCES_LESS ? 0 : (MainMenu.StartingResources == RESOURCES_NORMAL ? 50 : 250));
	Match.STARTING_WARRIORS = (MainMenu.StartingResources == RESOURCES_LESS ? 0 : (MainMenu.StartingResources == RESOURCES_NORMAL ? 10 : 40));;

	Match.UNITS_BEGIN = (Unit*)malloc(sizeof(Unit));
	Match.UNITS_END = (Unit*)malloc(sizeof(Unit));
	Match.UNITS_BEGIN->PREV = NULL;
	Match.UNITS_BEGIN->NEXT = Match.UNITS_END;
	Match.UNITS_END->NEXT = NULL;
	Match.UNITS_END->PREV = Match.UNITS_BEGIN;

	Match.SelectedEnemyTown = NULL;
	Match.EnemyTownToSelectAfterCapture = NULL;
	Match.SelectedTown = NULL;

	Match.TOWNS = Match.LAST_TOWN = (Town*)malloc(sizeof(Town));
	Match.TOWNS->NEXT = NULL;

	Town *this, *another;
	Coord MAP_MAX;
	MAP_MAX.X = MAP_MAX.Y = MainMenu.MapSize;

	int i, invalid, tries;
	for (i = 0; i < Match.NumberOfEnemies + 1; i++)
	{
		this = (Town*)malloc(sizeof(Town));
		this->peasants = Match.STARTING_PEASANTS;
		this->resources = Match.StartingResources;
		this->warriors = Match.STARTING_WARRIORS;
		this->NEXT = NULL;

		tries = 0;
		do
		{
			invalid = 0;

			this->Position.X = randmax(MAP_MAX.X);
			this->Position.Y = randmax(MAP_MAX.Y);

			another = Match.TOWNS;
			for (another = Match.TOWNS->NEXT; another != NULL; another = another->NEXT)
			{
				if (Match.MINIMUM_DISTANCE > sqrt(
									pow(another->Position.X - this->Position.X, 2) +
									pow(another->Position.Y - this->Position.Y, 2)
									))
				{
					invalid = 1;
					tries++;
				}
			}

			if (tries > 10)
			{
				MAP_MAX.X += 1;
				MAP_MAX.Y += 1;
				tries = 0;
			}

		} while (invalid);


		do
		{
			invalid = 0; // feltételezem, hogy jó színt fog generálni

			if (NOCOLOR != Match.PlayerColor && i == Match.NumberOfEnemies / 2) // ha pont a "középsőedik" falut hoztam létre, akkor az legyen a játékos faluja
			{
				this->Color = Match.PlayerColor;
				Match.SelectedTown = this;
			}
			else
			{
				//  különben random szín
				this->Color = rand() % 8;

				if (this->Color == Match.PlayerColor)
				{
					// ha a random szín pont a játékosé, akkor rövidzár és újra próbálkozás

					invalid = 1;
					continue;
				}
			}

			// ha megvan a random szín, akkor meg végigmegyek a többi falun
			for (another = Match.TOWNS->NEXT; another != NULL; another = another->NEXT)
			{
				// és ha a szín már foglalt
				if (another->Color == this->Color)
					invalid = 1;	// akkor bizony az invalid
			}

		} while (invalid);

		//this->Color = i;

		Match.LAST_TOWN->NEXT = this;
		Match.LAST_TOWN = this;
	}

	Town *t_it;
	int town_count = 0;
	Match.Center.X = Match.Center.Y = 0;
	for (t_it = Match.TOWNS->NEXT; t_it != NULL; t_it = t_it->NEXT)
	{
		Match.Center.X += t_it->Position.X;
		Match.Center.Y += t_it->Position.Y;
		++town_count;
	}
	Match.Center.X /= town_count;
	Match.Center.Y /= town_count;

	if (NULL != Match.SelectedTown)
	{
		Match.Viewport.X = Match.SelectedTown->Position.X * IMAGINARY_GRID_SIZE - Application.Output->w / 2;
		Match.Viewport.Y = Match.SelectedTown->Position.Y * IMAGINARY_GRID_SIZE - Application.Output->h / 2;
	}
	else
	{
		Match.Viewport.X = Match.Center.X * IMAGINARY_GRID_SIZE - Application.Output->w / 2;
		Match.Viewport.Y = Match.Center.Y * IMAGINARY_GRID_SIZE - Application.Output->h / 2;
	}

	Match.VIEWPORT_MIN.X = Match.TOWNS->NEXT->Position.X;
	Match.VIEWPORT_MIN.Y = Match.TOWNS->NEXT->Position.Y;

	Match.VIEWPORT_MAX.X = Match.TOWNS->NEXT->Position.X;
	Match.VIEWPORT_MAX.Y = Match.TOWNS->NEXT->Position.Y;

	for (this = Match.TOWNS->NEXT->NEXT; this != NULL; this = this->NEXT)
	{
		if (this->Position.X < Match.VIEWPORT_MIN.X)
			Match.VIEWPORT_MIN.X = this->Position.X;
		if (this->Position.Y < Match.VIEWPORT_MIN.Y)
			Match.VIEWPORT_MIN.Y = this->Position.Y;

		if (this->Position.X > Match.VIEWPORT_MAX.X)
			Match.VIEWPORT_MAX.X = this->Position.X;
		if (this->Position.Y > Match.VIEWPORT_MAX.Y)
			Match.VIEWPORT_MAX.Y = this->Position.Y;
	}

	Match.VIEWPORT_MIN.X = Match.VIEWPORT_MIN.X * IMAGINARY_GRID_SIZE + TOWN_TILE_SIZE * 0.5 - Application.Output->w;
	Match.VIEWPORT_MIN.Y = Match.VIEWPORT_MIN.Y * IMAGINARY_GRID_SIZE + TOWN_TILE_SIZE * 0.5 - Application.Output->h;

	Match.VIEWPORT_MAX.X = Match.VIEWPORT_MAX.X * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE * 0.5;
	Match.VIEWPORT_MAX.Y = Match.VIEWPORT_MAX.Y * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE * 0.5;
}

void STATE_PLAYING_UNINIT()
{
	/*
	**	FALUK FELTAKARÍTÁSA
	*/

	Town *t_it = Match.TOWNS, *t_tmp;

	while (t_it != NULL)
	{
		t_tmp = t_it->NEXT;
		free(t_it);
		t_it = t_tmp;
	}



	/*
	**	Unit-OK FELSZABADÍTÁSA
	*/

	Unit *u_it = Match.UNITS_BEGIN, *u_tmp;

	while (u_it != NULL)
	{
		u_tmp = u_it->NEXT;
		free(u_it);
		u_it = u_tmp;
	}
}


void STATE_PLAYING_LOOP()
{
	int repeat;
	while (SDL_PollEvent(&Application.e))
	{
		switch (Application.e.type)
		{
			case SDL_KEYDOWN:
				switch (Application.e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						if ((SDL_GetModState() & KMOD_CTRL) > 0)
							PROGRAM_SWITCH_STATE(STATE_MAIN_MENU);
					break;

					case SDLK_LEFT:
						KeyDown.Left = 1;
					break;

					case SDLK_RIGHT:
						KeyDown.Right = 1;
					break;

					case SDLK_UP:
						KeyDown.Up = 1;
					break;

					case SDLK_DOWN:
						KeyDown.Down = 1;
					break;

					case SDLK_HOME:
						for (repeat = 0; repeat < 10; ++repeat)
						{
							TownCreatePeasant(Match.SelectedTown);
						}
					break;

					case SDLK_END:
						for (repeat = 0; repeat < 10; ++repeat)
						{
							TownCreateWarrior(Match.SelectedTown);
						}
					break;

					case SDLK_PAGEUP:
						TownCreatePeasant(Match.SelectedTown);
					break;

					case SDLK_PAGEDOWN:
						TownCreateWarrior(Match.SelectedTown);
					break;

					case SDLK_c:
						if (NULL != Match.SelectedTown)
						{
							Match.Viewport.X = Match.SelectedTown->Position.X * IMAGINARY_GRID_SIZE - Application.Output->w / 2;
							Match.Viewport.Y = Match.SelectedTown->Position.Y * IMAGINARY_GRID_SIZE - Application.Output->h / 2;
						}
						else
						{
							Match.Viewport.X = Match.Center.X * IMAGINARY_GRID_SIZE - Application.Output->w / 2;
							Match.Viewport.Y = Match.Center.Y * IMAGINARY_GRID_SIZE - Application.Output->h / 2;
						}
					break;

					default: break;
				}
			break;

			case SDL_KEYUP:
				switch (Application.e.key.keysym.sym)
				{
					case SDLK_LEFT:
						KeyDown.Left = 0;
					break;

					case SDLK_RIGHT:
						KeyDown.Right = 0;
					break;

					case SDLK_UP:
						KeyDown.Up = 0;
					break;

					case SDLK_DOWN:
						KeyDown.Down = 0;
					break;

					default: break;
				}
			break;

			case SDL_MOUSEMOTION:
				Application.Cursor.X = Application.e.motion.x;
				Application.Cursor.Y = Application.e.motion.y;
			break;

			case SDL_MOUSEBUTTONDOWN:
			{
				Town *it;
				if (Application.e.button.button == SDL_BUTTON_LEFT)
				{
					Match.EnemyTownToSelectAfterCapture = NULL;
				}

				for (it = Match.TOWNS->NEXT; it != NULL; it = it->NEXT)
				{
					if ( it->Position.X * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.X < Application.e.button.x &&
						it->Position.X * IMAGINARY_GRID_SIZE + TOWN_TILE_SIZE / 2 - Match.Viewport.X > Application.e.button.x &&
						it->Position.Y * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.Y < Application.e.button.y &&
						it->Position.Y * IMAGINARY_GRID_SIZE + TOWN_TILE_SIZE / 2 - Match.Viewport.Y > Application.e.button.y )
					{

						if (Application.e.button.button == SDL_BUTTON_LEFT)
						{
							if (it->Color == Match.PlayerColor)
								Match.SelectedTown = it;
							else
								Match.EnemyTownToSelectAfterCapture = it;
						}

						if (Application.e.button.button == SDL_BUTTON_RIGHT)
						{
							if (NULL != Match.SelectedTown && Match.SelectedTown->warriors >= UNIT_SIZE)
							{
								TownSendUnit(Match.SelectedTown, it);
								Match.SelectedEnemyTown = it;
							}
						}
					}
				}
			}
			break;

			default: break;
		}
	}



	/*
	**	MAIN CYCLE
	*/

	Town *t_it;
	Unit *u_it, *u_tmp;

	// faluk bejárása, alapvető játékdinamika
	for (t_it = Match.TOWNS->NEXT; t_it != NULL; t_it = t_it->NEXT)
	{
		t_it->resources += t_it->peasants * (PEASANT_PRODUCTION_PER_MIN / 60.0) / Application.LPS_CAP;
		t_it->resources -= t_it->warriors * (WARRIOR_CONSUPTION_PER_MIN / 60.0) / Application.LPS_CAP;

		t_it->PRODUCTION = t_it->peasants * PEASANT_PRODUCTION_PER_MIN - t_it->warriors * WARRIOR_CONSUPTION_PER_MIN;

		if (t_it->resources < 0)
		{
			if (t_it->warriors > 0)
				t_it->warriors--;
			else
				t_it->resources = 0;
		}

		if (t_it->peasants < 0)
			t_it->peasants = 0;
	}


	// egységek bejárása és vizsgálata
	u_it = Match.UNITS_BEGIN->NEXT;
	while (u_it != Match.UNITS_END)
	{
		if (u_it->ARRIVE <= Application.LastRun)
		{
			// megérkezett

			if (u_it->TO->Color == u_it->Color)
			{
				// saját színű faluba

				u_it->TO->warriors += UNIT_SIZE;
			}
			else
			{
				// ellenséges faluba

				if (u_it->TO->warriors >= UNIT_SIZE)
				{
					// több védő katona volt, mint ahány támadó

					u_it->TO->warriors -= UNIT_SIZE;
				}
				else
				{
					// kevesebb katona volt, mint ahány támadó

					if (u_it->TO->warriors * PEASANT_WARRIOR_RATIO + u_it->TO->peasants > PEASANT_WARRIOR_RATIO * UNIT_SIZE)
					{
						// ha parasztokkal több

						u_it->TO->warriors = 0;
						u_it->TO->peasants = (u_it->TO->warriors * PEASANT_WARRIOR_RATIO + u_it->TO->peasants - PEASANT_WARRIOR_RATIO * UNIT_SIZE);
					}
					else
					{
						// ha parasztokkal is kevesebb

						// Ha ez volt a játékos kiválasztott faluja, akkor válasszunk ki neki egy másikat.
						if (u_it->TO->Color == Match.PlayerColor && Match.SelectedTown == u_it->TO)
						{
							Match.SelectedTown = NULL;
							for (t_it = Match.TOWNS->NEXT; t_it != NULL; t_it = t_it->NEXT)
							{
								if (t_it->Color == Match.PlayerColor && t_it != u_it->TO)
								{
									Match.SelectedTown = t_it;
									break;
								}
							}
						}

						u_it->TO->peasants = u_it->TO->warriors = UNIT_SIZE / 2;
						u_it->TO->Color = u_it->Color;

						if (Match.SelectedEnemyTown == u_it->TO)
							Match.SelectedEnemyTown = NULL;

						if (Match.EnemyTownToSelectAfterCapture == u_it->TO && u_it->Color == Match.PlayerColor)
						{
							Match.EnemyTownToSelectAfterCapture = NULL;
							Match.SelectedTown = u_it->TO;
						}
					}
				}
			}

			u_tmp = u_it->NEXT;
			u_it->PREV->NEXT = u_it->NEXT;
			u_it->NEXT->PREV = u_it->PREV;
			free(u_it);
			u_it = u_tmp;

			/*
			**	CHECK VICTORY/DEFEAT
			*/

			if (CheckPlayerVictory())
				PROGRAM_SWITCH_STATE(STATE_VICTORY);

			if (CheckPlayerDefeat())
				PROGRAM_SWITCH_STATE(STATE_DEFEAT);

		}
		else
		{
			u_it = u_it->NEXT;
		}
	}

	// mesterséges intelligencia minden egyes falun
	for (t_it = Match.TOWNS->NEXT; t_it != NULL; t_it = t_it->NEXT)
	{
		if (t_it->Color != Match.PlayerColor)
			AI(t_it);
	}









	/*
	**	RENDER
	*/


	// térképmozgatás
	if (KeyDown.Left)
		Match.Viewport.X -= VIEWPORT_MOVE_VALUE;

	if (KeyDown.Right)
		Match.Viewport.X += VIEWPORT_MOVE_VALUE;

	if (KeyDown.Up)
		Match.Viewport.Y -= VIEWPORT_MOVE_VALUE;

	if (KeyDown.Down)
		Match.Viewport.Y += VIEWPORT_MOVE_VALUE;


	// nézőpont beszorítása
	if (Match.Viewport.X < Match.VIEWPORT_MIN.X)
		Match.Viewport.X = Match.VIEWPORT_MIN.X;
	if (Match.Viewport.Y < Match.VIEWPORT_MIN.Y)
		Match.Viewport.Y = Match.VIEWPORT_MIN.Y;

	if (Match.Viewport.X > Match.VIEWPORT_MAX.X)
		Match.Viewport.X = Match.VIEWPORT_MAX.X;
	if (Match.Viewport.Y > Match.VIEWPORT_MAX.Y)
		Match.Viewport.Y = Match.VIEWPORT_MAX.Y;



	// fűkitöltés
	SDL_Rect rect;
	int x, y, t_y;
	x = (Match.Viewport.X % TOWN_TILE_SIZE + TOWN_TILE_SIZE) * -1;
	t_y = y = (Match.Viewport.Y % TOWN_TILE_SIZE + TOWN_TILE_SIZE) * -1;

	while (x < Application.Output->w)
	{
		y = t_y;

		while (y < Application.Output->h)
		{
			rect.x = x;
			rect.y = y;

			SDL_BlitSurface(Image.GRASS, NULL, Application.Output, &rect);

			y += TOWN_TILE_SIZE;
		}

		x += TOWN_TILE_SIZE;
	}


	// faluk megjelenítése
	t_it = Match.TOWNS;
	for (t_it = Match.TOWNS->NEXT; t_it != NULL; t_it = t_it->NEXT)
	{
		rect.x = t_it->Position.X * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.X;
		rect.y = t_it->Position.Y * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.Y;
		rect.w = TOWN_TILE_SIZE;
		rect.h = TOWN_TILE_SIZE;
		SDL_BlitSurface(Image.Town[t_it->Color], NULL, Application.Output, &rect);

		if (Match.SelectedTown == t_it)
		{
			rect.x = t_it->Position.X * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.X;
			rect.y = t_it->Position.Y * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.Y;
			SDL_BlitSurface(Image.SELECTED_TOWN_BORDER, NULL, Application.Output, &rect);
		}

		if (Match.EnemyTownToSelectAfterCapture == t_it)
		{
			rect.x = t_it->Position.X * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.X;
			rect.y = t_it->Position.Y * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.Y;
			SDL_BlitSurface(Image.ENEMY_TOWN_TO_SELECT_AFTER_CAPTURE_BORDER, NULL, Application.Output, &rect);
		}

		if (Match.SelectedEnemyTown == t_it)
		{
			rect.x = t_it->Position.X * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.X;
			rect.y = t_it->Position.Y * IMAGINARY_GRID_SIZE - TOWN_TILE_SIZE / 2 - Match.Viewport.Y;
			SDL_BlitSurface(Image.SELECTED_ENEMY_TOWN_BORDER, NULL, Application.Output, &rect);
		}
	}

	// egységek megjelenítése
	for (u_it = Match.UNITS_BEGIN->NEXT; u_it != Match.UNITS_END; u_it = u_it->NEXT)
	{
		double angle = atan2(u_it->TO->Position.Y - u_it->FROM->Position.Y, u_it->TO->Position.X - u_it->FROM->Position.X);
		double movement = (double)(Application.LastRun - u_it->LEAVE) / UNIT_MARCHING_TIME;
		double x = cos(angle) * movement;
		double y = sin(angle) * movement;

		SDL_Rect r;
		r.x = (x + u_it->FROM->Position.X) * IMAGINARY_GRID_SIZE - Match.Viewport.X - UNIT_CENTER_X;
		r.y = (y + u_it->FROM->Position.Y) * IMAGINARY_GRID_SIZE - Match.Viewport.Y - UNIT_CENTER_Y;
		SDL_BlitSurface(Image.Unit[u_it->Color], NULL, Application.Output, &r);
	}




	// faluk feliratainak megjelenítése
	t_it = Match.TOWNS;
	for (t_it = Match.TOWNS->NEXT; t_it != NULL; t_it = t_it->NEXT)
	{
		char chars[10];
		SDL_Rect town_on_screen;
		sprintf(chars, "%d", t_it->peasants);
		town_on_screen.x = t_it->Position.X * IMAGINARY_GRID_SIZE - Match.Viewport.X;
		town_on_screen.y = t_it->Position.Y * IMAGINARY_GRID_SIZE - Match.Viewport.Y;
		rect.x = town_on_screen.x + TOWN_OFFSET_PEASANT_X;
		rect.y = town_on_screen.y + TOWN_OFFSET_PEASANT_Y;
		TTF_RenderText_Outline(Application.Output, chars, Application.FONT, rect.x, rect.y, TextColor.WHITE, TextColor.BLACK);

		sprintf(chars, "%.0lf", t_it->resources);
		rect.x = town_on_screen.x + TOWN_OFFSET_RESOURCE_X;
		rect.y = town_on_screen.y + TOWN_OFFSET_RESOURCE_Y;
		TTF_RenderText_Outline(Application.Output, chars, Application.FONT, rect.x, rect.y, TextColor.WHITE, TextColor.BLACK);

		sprintf(chars, "%d", t_it->warriors);
		rect.x = town_on_screen.x + TOWN_OFFSET_WARRIOR_X;
		rect.y = town_on_screen.y + TOWN_OFFSET_WARRIOR_Y;
		TTF_RenderText_Outline(Application.Output, chars, Application.FONT, rect.x, rect.y, TextColor.WHITE, TextColor.BLACK);
	}




	// idő kiírása
	char bar[50];
	int seconds = (SDL_GetTicks() - Match.STARTED) / 1000;
	sprintf(bar, "%i:%02i", seconds / 60, seconds % 60);
	SDL_Surface *text = TTF_RenderText_Solid(Application.FONT, bar, TextColor.BLACK);
	SDL_Rect r;
	r.x = r.y = 10;
	SDL_BlitSurface(text, NULL, Application.Output, &r);
	SDL_FreeSurface(text);

	text = TTF_RenderText_Solid(Application.FONT, bar, TextColor.WHITE);
	r.x = r.y = 9;
	SDL_BlitSurface(text, NULL, Application.Output, &r);
	SDL_FreeSurface(text);



	/*
	**	"< PRESS CTRL+ESC TO RETURN TO MENU >"
	*/

	text = TTF_RenderText_Solid(Application.FONT, "< PRESS CTRL+ESC TO RETURN TO MENU >", TextColor.BLACK);
	r.x = Application.Output->w / 2 - text->w / 2;
	r.y = Application.Output->h - 30;
	SDL_BlitSurface(text, NULL, Application.Output, &r);
	SDL_FreeSurface(text);

	r.x = Application.Cursor.X;
	r.y = Application.Cursor.Y;

	SDL_BlitSurface(Image.HAND, NULL, Application.Output, &r);
}

void STATE_PLAYING_RESIZE()
{
	// Következő LOOP mindent jól újrarajzol.
}
