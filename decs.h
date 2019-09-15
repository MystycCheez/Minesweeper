#ifndef _decs_h_
#define _decs_h_

#include "SDL_structs.h"

void Initiate();

Tilelist gameStart(Difficulty d);
Difficulty setDiff();

Textures TextureHandler();

SDL_Window* WindowInit();
SDL_Renderer* rendererInit(SDL_Window* window);

Field FieldInit(Difficulty d);
//Field FieldUpdate(Field field);

#endif