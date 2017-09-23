#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "globals.h"
#include "game_methods.h"

void AI(Town *this)
{
	if (randmax(Application.LPS_CAP) > 10)
		return;

	if (randmax(100) < Match.Difficulty && this->resources > WARRIOR_COST && this->peasants * WARRIOR_CONSUPTION_PER_MIN / PEASANT_PRODUCTION_PER_MIN > this->warriors)
		TownCreateWarrior(this);
	else
		if (randmax(100) < Match.Difficulty) TownCreatePeasant(this);

	Town *target = Match.TOWNS;
	int num = randmax(Match.NumberOfEnemies + 1) + 1;
	while (num--)
		target = target->NEXT;

	if (randmax(100) < Match.Difficulty && target->Color != this->Color && target != NULL)
		TownSendUnit(this, target);
}

void TTF_RenderText_Outline(SDL_Surface *dst, char *string, TTF_Font *font, int X, int Y, SDL_Color Ctxt, SDL_Color Cout)
{
	SDL_Surface *text;
	SDL_Rect r;

	text = TTF_RenderText_Solid(font, string, Cout);
	r.x = X - 1 - text->w / 2;
	r.y = Y - 1 - text->h / 2;
	SDL_BlitSurface(text, NULL, dst, &r);

	r.x = X + 1 - text->w / 2;
	r.y = Y + 1 - text->h / 2;
	SDL_BlitSurface(text, NULL, dst, &r);

	r.x = X - 1 - text->w / 2;
	r.y = Y + 1 - text->h / 2;
	SDL_BlitSurface(text, NULL, dst, &r);

	r.x = X + 1 - text->w / 2;
	r.y = Y - 1 - text->h / 2;
	SDL_BlitSurface(text, NULL, dst, &r);
	SDL_FreeSurface(text);

	text = TTF_RenderText_Solid(font, string, Ctxt);
	r.x = X - text->w / 2;
	r.y = Y - text->h / 2;
	SDL_BlitSurface(text, NULL, dst, &r);
	SDL_FreeSurface(text);
}

void TownCreatePeasant(Town *this)
{
	/*
	**	PARASZT KÉSZÍTÉS AZ ADOTT FALUBAN
	*/

	if (NULL == this)
	{
		return;
	}

	if (this->resources >= PEASANT_COST)
	{
		this->peasants += 1;
		this->resources -= PEASANT_COST;
	}
}

void TownCreateWarrior(Town *this)
{
	/*
	**	KATONA KÉSZÍTÉS AZ ADOTT FALUBAN
	*/

	if (NULL == this)
	{
		return;
	}

	if (this->resources >= WARRIOR_COST && this->peasants * PEASANT_PRODUCTION_PER_MIN >= (this->warriors + 1) * WARRIOR_CONSUPTION_PER_MIN)
	{
		this->warriors += 1;
		this->peasants -= 1;
		this->resources -= WARRIOR_COST;
	}
}



void TownSendUnit(Town *from, Town *to)
{
	if (NULL == from || NULL == to)
	{
		return;
	}

	if (from->warriors < UNIT_SIZE)
		return;

	/*
	**	KATONÁK KIKÜLDÉSE EGY FALUBÓL EGY MÁSIKBA
	*/

	Unit *prev = Match.UNITS_END->PREV;

	Unit *this = (Unit*)malloc(sizeof(Unit));
	this->PREV = prev;
	this->NEXT = Match.UNITS_END;
	prev->NEXT = this;
	Match.UNITS_END->PREV = this;

	this->FROM = from;
	this->TO = to;
	this->Color = from->Color;
	this->LEAVE = Application.LastRun;
	this->ARRIVE = this->LEAVE + (sqrt((from->Position.X - to->Position.X) * (from->Position.X - to->Position.X) + (from->Position.Y - to->Position.Y) * (from->Position.Y - to->Position.Y))) * UNIT_MARCHING_TIME;

	from->warriors -= UNIT_SIZE;
}

int CheckPlayerVictory()
{
	/*
	**	"JÁTÉKOS NYERT-E" FGV., 1-ET AD VISSZA, HA NYERT, MINDEN MÁS ESETBEN 0-T
	*/


	if (NOCOLOR == Match.PlayerColor)
	{
		return 0;
	}

	Town *t;
	for (t = Match.TOWNS->NEXT; t != NULL; t = t->NEXT)
		if (t->Color != Match.PlayerColor)
			return 0;

	Unit *u;
	for (u = Match.UNITS_BEGIN->NEXT; u != Match.UNITS_END; u = u->NEXT)
		if (u->Color != Match.PlayerColor)
			return 0;

	return 1;
}

int CheckPlayerDefeat()
{
	/*
	**	"JÁTÉKOS VESZTETT-E" FGV., 1-ET AD VISSZA, HA IGEN, MINDEN MÁS ESETBEN 0-T
	*/

	if (NOCOLOR == Match.PlayerColor)
	{
		return 0;
	}

	Town *t;
	for (t = Match.TOWNS->NEXT; t != NULL; t = t->NEXT)
		if (t->Color == Match.PlayerColor)
			return 0;

	Unit *u;
	for (u = Match.UNITS_BEGIN->NEXT; u != Match.UNITS_END; u = u->NEXT)
		if (u->Color == Match.PlayerColor)
			return 0;

	return 1;
}
