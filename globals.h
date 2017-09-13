#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

typedef enum
{
	PLAYER_NAME_LENGTH = 21, // maximális hossz
	VIEWPORT_MOVE_VALUE = 20, // térkép ugrásának mértéke renderelésenként
	TOWN_TILE_SIZE = 64, // falut jelképező kép mérete
	UNIT_CENTER_X = 10, // egységek jelképező kép közepének x coordinátája
	UNIT_CENTER_Y = 15, // egységek jelképező kép közepének y coordinátája
	IMAGINARY_GRID_SIZE = 16, // négyzetrács mérete, erre vannak rárakva a faluk
	TOWN_OFFSET_RESOURCE_X = -12, // falu bal felső sarkától merre legyen az erőforrás szám megjelenítve
	TOWN_OFFSET_RESOURCE_Y = 24,
	TOWN_OFFSET_PEASANT_X = 13, // falu bal felső sarkától merre legyen a paraszt létszám megjelenítve
	TOWN_OFFSET_PEASANT_Y = 17,
	TOWN_OFFSET_WARRIOR_X = -20, // falu bal felső sarkától merre legyen az katona létszám megjelenítve
	TOWN_OFFSET_WARRIOR_Y = -22,

	PEASANT_PRODUCTION_PER_MIN = 30, // parasztok termelése
	PEASANT_COST = 10, // paraszt készítés ára
	WARRIOR_CONSUPTION_PER_MIN = 10, // katonák fogyasztása
	WARRIOR_COST = 20, // katona készítés ára
	UNIT_SIZE = 10, // zászlóalj mérete
	UNIT_MARCHING_TIME = 1000,	// egy egységnyi út megtételéhez szükséges idő ms-ben
	PEASANT_WARRIOR_RATIO = 3, // haderő számításához, egy katona hány paraszttal ér fel

	MAIN_MENU_WIDTH = 250, // főmenü szélessége pixelben
	MAIN_MENU_Y = 315, // főmenü függőleges elhelyezkedése

	HIGHSCORES_WIDTH = 400
} GLOBALS;

typedef struct
{
	char name[PLAYER_NAME_LENGTH];
	int score;
} Highscore;

typedef struct
{	// lenyomott kurzorbillentyűk állapotának tárolásához (térképmozgatásnál)

	int Left, Right, Up, Down;
} KeysDown;

typedef struct
{	// pár szövegszín tárolása egyszerűbb elérés érdekében

	SDL_Color LIGHT_GRAY, WHITE, GOLD, RED, BLACK;
} TextColors;

typedef struct
{	// koordináta-párt tároló struct

	int X, Y;
} Coord;

typedef enum
{	// alapvető játszható színek tárolásához

	RED, YELLOW, GREEN, BLUE, PURPLE, PINK, BLACK, WHITE
} Color;

typedef struct
{
	Uint32 INT[8];
} ColorValues;
ColorValues ColorValue;



typedef void (*STATE_FUNC)(void); // állapot függvény typedef, void-ot vár és void-ot ad
typedef struct
{
	STATE_FUNC INIT;
	STATE_FUNC LOOP;
	STATE_FUNC RESIZE;
	STATE_FUNC UNINIT;
} PROGRAM_STATE; // egyes állapotok struktúrái

typedef struct
{	// program legalapvetőbb tulajdonságai

	SDL_Surface* Output; // videokimenet
	SDL_Event e; // események tárolására
	int LastRun, LPS_CAP; // utolsó lefutás ideje és lefutások számának és idejének korlátozása
	Coord Cursor;
	PROGRAM_STATE *State, *NextState; // program állapota
	TTF_Font *FONT, *FONT_MORPHEUS, *FONT_MORPHEUS_BIG; // két betöltött betűtípus
} ApplicationProperties;


typedef enum
{	// főmenü állapotai - melyik menüpont az aktív

	STATE_MAIN_MENU_PLAYER_NAME,
	STATE_MAIN_MENU_PLAYER_COLOR,
	STATE_MAIN_MENU_NUMBER_OF_ENEMIES,
	STATE_MAIN_MENU_DIFFICULTY,
	STATE_MAIN_MENU_MAP_SIZE,
	STATE_MAIN_MENU_STARTING_RESORUCES,

	STATE_MAIN_MENU_VIEW_HIGHSCORES
} MainMenuState;
typedef enum
{	// főmenü-beli választható nehézségek

	DIFFICULTY_EASY = 20,
	DIFFICULTY_NORMAL = 50,
	DIFFICULTY_HARD = 85
} MainMenuDifficulty;
typedef enum
{	// főmenü-beli választható térképméret

	MAP_SIZE_SMALL = 7,
	MAP_SIZE_NORMAL = 14,
	MAP_SIZE_BIG = 21
} MainMenuMapSize;
typedef enum
{	// főmenü-beli választható kezdeti mennyiségek

	RESOURCES_LESS = 1,
	RESOURCES_NORMAL = 2,
	RESOURCES_MORE = 3
} MainMenuStartingResources;
typedef struct
{	// főmenü aktuális állapotának tárolására

	MainMenuState State; // melyik menüpont az aktív

	char PlayerName[PLAYER_NAME_LENGTH]; // mi a begépelt játékosnév

	Color PlayerColor; // játékos választott színe
	int NumberOfEnemies; // választott ellenségek száma
	MainMenuDifficulty Difficulty; // választott nehézség
	MainMenuMapSize MapSize; // választott térképméret
	MainMenuStartingResources StartingResources; // választott kezdeti mennyiségek
} MainMenuProperties;



typedef struct
{
	// képek betöltésére és elérésére

	SDL_Surface *Town[8], *Unit[8], *MAIN_MENU_BG, *SELECTED_TOWN_BORDER, *SELECTED_ENEMY_TOWN_BORDER, *GRASS, *HAND;
} Images;


typedef struct Town
{
	// egy adott falu adatait tartalmazó struct

	Coord Position; // falu helye
	int peasants, // parasztok és katonák száma
	    warriors;
	double resources; // nyersanyagok száma - double, mivel a termelés nem egész
	double PRODUCTION; // termelékenység
	Color Color; // falu színe

	struct Town *NEXT; // következő falura mutató pointer - mivel láncolt listát képezek a falukból
} Town;

typedef struct Unit
{
	// menetelő zászlóalj tulajdonságait leíro struct

	Town *FROM, *TO; // melyik faluból melyik faluba
	int LEAVE, ARRIVE; // mikor hagyta el a faluját és mikor fog megérkezni
	Color Color; // zászlóalj színe

	struct Unit *NEXT, *PREV; // előző és következő zászlóaljra ptr - duplán linkelt láncolt listát alkalmazok, így könnyebb lista belsejéből törölni elemet
} Unit;

typedef struct
{
	// játszma tuladonságait összefoglaló struct

	int STARTED;

	Coord Viewport, VIEWPORT_MAX, VIEWPORT_MIN; // nézőpont tárolása és a két szélsőértéké (ezek közé szorítom pe mindig a nézőpontot)
	int NumberOfEnemies, // ellenségek száma
	    Difficulty, // nehézség
	    MINIMUM_DISTANCE, // minimális távolság a falvak között
	    STARTING_PEASANTS, // kezdetei parasztok, nyersanyagok és katonák száma
	    StartingResources,
	    STARTING_WARRIORS;

	Town *TOWNS, *LAST_TOWN; // falukból álló láncolt lista kezdete (strázsa) és az utolsó elem helye

	Unit *UNITS_BEGIN, *UNITS_END; // egységekből álló láncolz lista eleje és vége, mindkettő strázsa, duplán linkelt lista

	Color PlayerColor; // játékos színe
	char PlayerName[PLAYER_NAME_LENGTH + 1]; // játékos neve
	Town *SelectedTown, *SelectedEnemyTown; // kiválasztott saját és ellenséges falu
} MatchProperties;



extern KeysDown KeyDown;
extern TextColors TextColor;
extern ApplicationProperties Application;
extern MainMenuProperties MainMenu;
extern Images Image;
extern MatchProperties Match;
extern PROGRAM_STATE *STATE_MAIN_MENU,
					 *STATE_VIEW_HIGHSCORES,
					 *STATE_PLAYING,
					 *STATE_VICTORY,
					 *STATE_DEFEAT,
					 *STATE_QUITTING;

void PROGRAM_SWITCH_STATE(PROGRAM_STATE *State); // program fő állapotának váltására szolgáló fgv.
int randmax(int);

SDL_Surface *IMG_Load_Optimized(char *filename);

#endif
