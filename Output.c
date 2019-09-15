#include <SDL.h>
#include "SDL_structs.h"
#include "misc.h"

const int SCR_WID = 480;
const int SCR_HGT = 270;


SDL_Window *WindowInit()
{
	SDL_Window* mainWindow = SDL_CreateWindow("test",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WID, SCR_HGT, SDL_WINDOW_SHOWN);
	return mainWindow;
}
SDL_Renderer* rendererInit(SDL_Window *window)
{
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	return renderer;
}
void Initiate()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}
Textures TextureHandler()
{
	Textures textures;
	textures.textures = calloc(11, sizeof(SDL_Surface));

	//SDL_SetSurfaceBlendMode(textures.textures[1], SDL_BLENDMODE_NONE);

	textures.textures[0] = SDL_LoadBMP("textures\\tile_unclicked.bmp");
	textures.textures[1] = SDL_LoadBMP("textures\\tile_checked.bmp");

	textures.textures[2] = SDL_LoadBMP("textures\\bomb.bmp");
	textures.textures[3] = SDL_LoadBMP("textures\\tile_1.bmp");
	textures.textures[4] = SDL_LoadBMP("textures\\tile_2.bmp");
	textures.textures[5] = SDL_LoadBMP("textures\\tile_3.bmp");
	textures.textures[6] = SDL_LoadBMP("textures\\tile_4.bmp");
	textures.textures[7] = SDL_LoadBMP("textures\\tile_5.bmp");
	textures.textures[8] = SDL_LoadBMP("textures\\tile_6.bmp");
	textures.textures[9] = SDL_LoadBMP("textures\\tile_7.bmp");
	textures.textures[10] = SDL_LoadBMP("textures\\tile_8.bmp");

	for (int i = 2; i <= 10; i++)
	{
		//SDL_SetSurfaceBlendMode(textures.textures[i], SDL_BLENDMODE_NONE);
		int key = SDL_MapRGB(textures.textures[i]->format, 0, 255, 255);
		SDL_SetColorKey(textures.textures[i], SDL_TRUE, key);
	}
	/*SDL_Surface *bomb = textures.textures[0];
	SDL_Surface *tileUntoggled = textures.textures[1];
	SDL_Surface *tileToggled = textures.textures[2];*/



	return textures;
}
Field FieldInit(Difficulty d)
{
	Field field;
	field.tiles = calloc(d.columns * d.rows, sizeof(SDL_Rect));
	for (int i = 0; i < d.columns * d.rows; i++)
	{
		field.tiles[i].h = 16;
		field.tiles[i].w = 16;

	}

	for (int i = 0; i < d.rows; i++)
	{
		for (int w = 0; w < d.columns; w++)
		{
			if (w + (i * d.columns) < d.columns * d.rows)
			{
				field.tiles[w + (i * d.columns)].x = w * 16;
				field.tiles[w + (i * d.columns)].y = i * 16;
			}
		}
	}

	return field;
}