#include "globals.h"
#include "state_main_menu.h"

void STATE_MAIN_MENU_INIT()
{
	SDL_FillRect(Application.Output, NULL, SDL_MapRGB(Application.Output->format, 0, 0, 0));
	return;
}

void STATE_MAIN_MENU_UNINIT()
{
	// nincs mit feltakarítani

	return;
}

void STATE_MAIN_MENU_LOOP()
{
	SDL_Rect rect;
	Uint32 rect_color;
	SDL_Surface *text = NULL;


	/*
	**	MENU BACKGROUND
	*/

	rect.x = Application.Output->w / 2 - Image.MAIN_MENU_BG->w / 2; // vízszintesen középre
	rect.y = 0;
	SDL_BlitSurface(Image.MAIN_MENU_BG, NULL, Application.Output, &rect);


	/*
	**	FELHASZNÁLÓ NEVE
	*/

	rect.w = 250;
	rect.h = 50;
	rect.x = Application.Output->w / 2 - rect.w / 2;
	rect.y = MAIN_MENU_Y;
	if (MainMenu.State == STATE_MAIN_MENU_PLAYER_NAME)
	{
		rect_color = SDL_MapRGB(Application.Output->format, 50, 50, 50);
	}
	else
	{
		rect_color = SDL_MapRGB(Application.Output->format, 30, 30, 30);
	}
	SDL_FillRect(Application.Output, &rect, rect_color);

	if (strlen(MainMenu.PlayerName) > 0)
	{
		/*
		**	HA A FELHASZNÁLÓ NEVE NEM ÜRES (különben behal, mert nincs renderelendő string)
		*/

		text = TTF_RenderText_Blended(Application.FONT_MORPHEUS, MainMenu.PlayerName, TextColor.GOLD);
		rect.x = Application.Output->w / 2 - text->w / 2;
		rect.y = MAIN_MENU_Y;
		rect.w = rect.h = 0;
		SDL_BlitSurface(text, NULL, Application.Output, &rect);
		SDL_FreeSurface(text);
	}


	/*
	**	MENÜPONT:	Color
	*/

	text = TTF_RenderText_Solid(Application.FONT, "Color:", MainMenu.State == STATE_MAIN_MENU_PLAYER_COLOR ? TextColor.WHITE : TextColor.LIGHT_GRAY);
	rect.x = Application.Output->w / 2 - MAIN_MENU_WIDTH / 2;
	rect.y = MAIN_MENU_Y + 50;
	SDL_BlitSurface(text, NULL, Application.Output, &rect);
	SDL_FreeSurface(text);

	if (NOCOLOR != MainMenu.PlayerColor)
	{
		// választott érték
		text = TTF_RenderText_Solid(Application.FONT, "< x >", MainMenu.State == STATE_MAIN_MENU_PLAYER_COLOR ? TextColor.GOLD : TextColor.LIGHT_GRAY);
		rect.x = Application.Output->w / 2 + MAIN_MENU_WIDTH / 2 - text->w;
		SDL_BlitSurface(text, NULL, Application.Output, &rect);
		SDL_FreeSurface(text);

		// kis négyzet ráfestése
		SDL_Rect rect2;
		rect2.w = rect2.h = 10;
		rect2.x = Application.Output->w / 2 + MAIN_MENU_WIDTH / 2 - 21;
		rect2.y = rect.y + 3;
		SDL_FillRect(Application.Output, &rect2, ColorValue.INT[MainMenu.PlayerColor]);
	}
	else
	{
		text = TTF_RenderText_Solid(Application.FONT, "< spectator >", MainMenu.State == STATE_MAIN_MENU_PLAYER_COLOR ? TextColor.GOLD : TextColor.LIGHT_GRAY);
		rect.x = Application.Output->w / 2 + MAIN_MENU_WIDTH / 2 - text->w;
		SDL_BlitSurface(text, NULL, Application.Output, &rect);
		SDL_FreeSurface(text);
	}




	/*
	**	MENÜPONT:	NUMBER OF ENEMIES
	*/

	text = TTF_RenderText_Solid(Application.FONT, "Number of enemies:", MainMenu.State == STATE_MAIN_MENU_NUMBER_OF_ENEMIES ? TextColor.WHITE : TextColor.LIGHT_GRAY);
	rect.x = Application.Output->w / 2 - MAIN_MENU_WIDTH / 2;
	rect.y += 20;
	SDL_BlitSurface(text, NULL, Application.Output, &rect);
	SDL_FreeSurface(text);

		// választott érték
		char number[] = "<   >";
		number[2] = MainMenu.NumberOfEnemies + '0';
		text = TTF_RenderText_Solid(Application.FONT, number, MainMenu.State == STATE_MAIN_MENU_NUMBER_OF_ENEMIES ? TextColor.GOLD : TextColor.LIGHT_GRAY);
		rect.x = Application.Output->w / 2 + MAIN_MENU_WIDTH / 2 - text->w;
		SDL_BlitSurface(text, NULL, Application.Output, &rect);
		SDL_FreeSurface(text);




	/*
	**	MENÜPONT:	Difficulty
	*/

	text = TTF_RenderText_Solid(Application.FONT, "Difficulty:", MainMenu.State == STATE_MAIN_MENU_DIFFICULTY ? TextColor.WHITE : TextColor.LIGHT_GRAY);
	rect.x = Application.Output->w / 2 - MAIN_MENU_WIDTH / 2;
	rect.y += 20;
	SDL_BlitSurface(text, NULL, Application.Output, &rect);
	SDL_FreeSurface(text);

		// választott érték
		text = TTF_RenderText_Solid(Application.FONT, MainMenu.Difficulty == DIFFICULTY_EASY ? "< easy >" : (MainMenu.Difficulty == DIFFICULTY_NORMAL ? "< normal >" : "< hard >"), MainMenu.State == STATE_MAIN_MENU_DIFFICULTY ? TextColor.GOLD : TextColor.LIGHT_GRAY);
		rect.x = Application.Output->w / 2 + MAIN_MENU_WIDTH / 2 - text->w;
		SDL_BlitSurface(text, NULL, Application.Output, &rect);
		SDL_FreeSurface(text);








	/*
	**	MENÜPONT:	MAP SIZE
	*/

	text = TTF_RenderText_Solid(Application.FONT, "Map Size:", MainMenu.State == STATE_MAIN_MENU_MAP_SIZE ? TextColor.WHITE : TextColor.LIGHT_GRAY);
	rect.x = Application.Output->w / 2 - MAIN_MENU_WIDTH / 2;
	rect.y += 20;
	SDL_BlitSurface(text, NULL, Application.Output, &rect);
	SDL_FreeSurface(text);

		// választott érték
		text = TTF_RenderText_Solid(Application.FONT, MainMenu.MapSize == MAP_SIZE_SMALL ? "< small >" : (MainMenu.MapSize == MAP_SIZE_NORMAL ? "< normal >" : "< big >"), MainMenu.State == STATE_MAIN_MENU_MAP_SIZE ? TextColor.GOLD : TextColor.LIGHT_GRAY);
		rect.x = Application.Output->w / 2 + MAIN_MENU_WIDTH / 2 - text->w;
		SDL_BlitSurface(text, NULL, Application.Output, &rect);
		SDL_FreeSurface(text);




	/*
	**	MENÜPONT:	STARTING RESOURCES
	*/

	text = TTF_RenderText_Solid(Application.FONT, "Starting resources:", MainMenu.State == STATE_MAIN_MENU_STARTING_RESORUCES ? TextColor.WHITE : TextColor.LIGHT_GRAY);
	rect.x = Application.Output->w / 2 - MAIN_MENU_WIDTH / 2;
	rect.y += 20;
	SDL_BlitSurface(text, NULL, Application.Output, &rect);
	SDL_FreeSurface(text);

		// választott érték
		text = TTF_RenderText_Solid(Application.FONT, MainMenu.StartingResources == RESOURCES_LESS ? "< less >" : ( MainMenu.StartingResources == RESOURCES_NORMAL ? "< normal >" : "< more >"), MainMenu.State == STATE_MAIN_MENU_STARTING_RESORUCES ? TextColor.GOLD : TextColor.LIGHT_GRAY);
		rect.x = Application.Output->w / 2 + MAIN_MENU_WIDTH / 2 - text->w;
		SDL_BlitSurface(text, NULL, Application.Output, &rect);
		SDL_FreeSurface(text);


	/*
	**	MENÜPONT:	VIEW HIGHSCORES
	*/

	text = TTF_RenderText_Solid(Application.FONT, "View highscores", MainMenu.State == STATE_MAIN_MENU_VIEW_HIGHSCORES ? TextColor.WHITE : TextColor.LIGHT_GRAY);
	rect.y += 67;
	rect.x = Application.Output->w / 2 - text->w / 2;
	SDL_BlitSurface(text, NULL, Application.Output, &rect);
	SDL_FreeSurface(text);





	/*
	**	"< PRESS CTRL+ESC TO RETURN TO MENU >"
	*/

	text = TTF_RenderText_Solid(Application.FONT, "< PRESS ESC TO QUIT >", TextColor.BLACK);
	rect.x = Application.Output->w / 2 - text->w / 2;
	rect.y = Application.Output->h - 30;
	SDL_BlitSurface(text, NULL, Application.Output, &rect);
	SDL_FreeSurface(text);




	/*
	**	SOK RENDERELÉS UTÁN FLIP, MEG ESEMÉNYRE VÁRÁS
	*/

	SDL_Flip(Application.Output);
	SDL_WaitEvent(&Application.e);


	/*
	**	ESEMÉNY FELDOLGOZÁS
	*/

	switch (Application.e.type)
	{
		case SDL_KEYDOWN:
				switch (Application.e.key.keysym.sym)
				{
					case SDLK_RETURN:
						if (MainMenu.State == STATE_MAIN_MENU_VIEW_HIGHSCORES)
							PROGRAM_SWITCH_STATE(STATE_VIEW_HIGHSCORES);
						else
							PROGRAM_SWITCH_STATE(STATE_PLAYING);
					break;

					case SDLK_ESCAPE:
						PROGRAM_SWITCH_STATE(STATE_QUITTING);
					break;

					case SDLK_BACKSPACE:
						if (!MainMenu.PlayerTypedAName)
						{
							MainMenu.PlayerTypedAName = 1;
							MainMenu.PlayerName[0] = '\0';
						}
						if (strlen(MainMenu.PlayerName) > 0)
						{
							MainMenu.PlayerName[strlen(MainMenu.PlayerName) - 1] = '\0';
						}
					break;

					case SDLK_DOWN:
						if (MainMenu.State < STATE_MAIN_MENU_VIEW_HIGHSCORES)
							MainMenu.State += 1;
					break;

					case SDLK_UP:
						if (MainMenu.State > STATE_MAIN_MENU_PLAYER_NAME)
							MainMenu.State -= 1;
					break;

					case SDLK_LEFT:
						switch (MainMenu.State)
						{
							case STATE_MAIN_MENU_PLAYER_COLOR:
								if (MainMenu.PlayerColor > RED)
									MainMenu.PlayerColor--;
							break;

							case STATE_MAIN_MENU_NUMBER_OF_ENEMIES:
								if (MainMenu.NumberOfEnemies > 1)
									MainMenu.NumberOfEnemies--;
							break;

							case STATE_MAIN_MENU_DIFFICULTY:
								if (MainMenu.Difficulty == DIFFICULTY_NORMAL)
									MainMenu.Difficulty = DIFFICULTY_EASY;

								if (MainMenu.Difficulty == DIFFICULTY_HARD)
									MainMenu.Difficulty = DIFFICULTY_NORMAL;
							break;

							case STATE_MAIN_MENU_MAP_SIZE:
								if (MainMenu.MapSize == MAP_SIZE_NORMAL)
									MainMenu.MapSize = MAP_SIZE_SMALL;

								if (MainMenu.MapSize == MAP_SIZE_BIG)
									MainMenu.MapSize = MAP_SIZE_NORMAL;
							break;

							case STATE_MAIN_MENU_STARTING_RESORUCES:
								if (MainMenu.StartingResources == RESOURCES_NORMAL)
									MainMenu.StartingResources = RESOURCES_LESS;

								if (MainMenu.StartingResources == RESOURCES_MORE)
									MainMenu.StartingResources = RESOURCES_NORMAL;
							break;

							default: break;
						}
					break;

					case SDLK_RIGHT:
						switch (MainMenu.State)
						{
							case STATE_MAIN_MENU_PLAYER_COLOR:
								if (MainMenu.PlayerColor < NOCOLOR)
									MainMenu.PlayerColor++;
							break;

							case STATE_MAIN_MENU_NUMBER_OF_ENEMIES:
								if (MainMenu.NumberOfEnemies < 7)
									MainMenu.NumberOfEnemies++;
							break;

							case STATE_MAIN_MENU_DIFFICULTY:
								if (MainMenu.Difficulty == DIFFICULTY_NORMAL)
									MainMenu.Difficulty = DIFFICULTY_HARD;

								if (MainMenu.Difficulty == DIFFICULTY_EASY)
									MainMenu.Difficulty = DIFFICULTY_NORMAL;
							break;

							case STATE_MAIN_MENU_MAP_SIZE:
								if (MainMenu.MapSize == MAP_SIZE_NORMAL)
									MainMenu.MapSize = MAP_SIZE_BIG;

								if (MainMenu.MapSize == MAP_SIZE_SMALL)
									MainMenu.MapSize = MAP_SIZE_NORMAL;
							break;

							case STATE_MAIN_MENU_STARTING_RESORUCES:
								if (MainMenu.StartingResources == RESOURCES_NORMAL)
									MainMenu.StartingResources = RESOURCES_MORE;

								if (MainMenu.StartingResources == RESOURCES_LESS)
									MainMenu.StartingResources = RESOURCES_NORMAL;
							break;

							default: break;
						}
					break;

					default:
						if (MainMenu.State == STATE_MAIN_MENU_PLAYER_NAME && (Application.e.key.keysym.sym == ' ' || (Application.e.key.keysym.sym >= 'a' && Application.e.key.keysym.sym <= 'z') || (Application.e.key.keysym.sym >= '0' && Application.e.key.keysym.sym <= '9')))
						{
							if (!MainMenu.PlayerTypedAName)
							{
								MainMenu.PlayerTypedAName = 1;
								MainMenu.PlayerName[0] = '\0';
							}
							int len = strlen(MainMenu.PlayerName);

							if (len < PLAYER_NAME_LENGTH)
							{
								MainMenu.PlayerName[len] = (((SDL_GetModState() & KMOD_SHIFT) > 0) ? toupper(Application.e.key.keysym.sym) : Application.e.key.keysym.sym);
								MainMenu.PlayerName[len + 1] = '\0';
							}
						}
					break;
				}
		break;

		case SDL_QUIT:
			PROGRAM_SWITCH_STATE(STATE_QUITTING);
		break;

		default: break;
	}
}

void STATE_MAIN_MENU_RESIZE()
{
	// Következő LOOP mindent jól újrarajzol.
}
