#ifndef _GAME_METHODS_H_
#define _GAME_METHODS_H_


void TownCreatePeasant(Town *this);

void TownCreateWarrior(Town *this);

void TownSendUnit(Town *from, Town *to);

int CheckPlayerVictory();

int CheckPlayerDefeat();

void AI(Town *this);

void TTF_RenderText_Outline(SDL_Surface *dst, char *string, TTF_Font *font, int X, int Y, SDL_Color Ctxt, SDL_Color Cout);


#endif
