#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <Windows.h>
#include <time.h>
//#include <setjmp.h>
//#include "Output.c"
#include <SDL.h>
#include "misc.h"
#include "defs.h"
#include "decs.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

//static jmp_buf buf;

int main()
{
	Tilelist tileList = gameStart(setDiff());

	SDL_Window *window = WindowInit();
	SDL_Renderer *renderer = rendererInit(window);

	SDL_Surface *screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00));


	SDL_UpdateWindowSurface(window);

	Textures textures = TextureHandler();

	Field field = FieldInit(medium);

	for (int i = 0; i < tileList.tileCount; i++)
	{
		SDL_BlitSurface(textures.textures[0], NULL, screen, &field.tiles[i]);
	}

	//SDL_Surface *test = SDL_LoadBMP("textures\\tile_unclicked.bmp");	//testing purposes
	//SDL_SaveBMP(test, "textures\\tile_unclicked2.bmp");				//ditto

	//SDL_BlitSurface(test, NULL, screen, NULL);						//ditto

	SDL_UpdateWindowSurface(window);

	//SDL_Delay(2000);

	int wid = 0;
	int hgt = 0;

	while (1)
	{
		SDL_MouseButtonEvent event;
		while (SDL_PollEvent(&event))
		{
			if ((event.button == SDL_BUTTON_LEFT) && (event.state == SDL_PRESSED))
			{
				for (int w = 0; w < tileList.width; w++)
				{
					if ((event.x >= field.tiles[w].x) && (event.x < field.tiles[w].x + 16))
					{
						wid = w;
						w = tileList.width;
					}
				}
				for (int h = 0; h < tileList.tileCount; h += tileList.width)
				{
					if ((event.y >= field.tiles[h].y) && (event.y < field.tiles[h].y + 16))
					{
						hgt = h;
						h = tileList.tileCount;
					}
				}

				SDL_BlitSurface(textures.textures[1], NULL, screen, &field.tiles[wid + hgt]);

				/*for (int i = 0; i < tileList.tileCount; i++)
				{
					int index[8];				// 3x3 with center missing
					//index[0] = i - tileList.width - 1;	//top left
					index[1] = i - tileList.width;		//top
					//index[2] = i - tileList.width + 1;	//top right
					index[3] = i - 1;				//left
					index[4] = i + 1;				//right
					//index[5] = i + tileList.width - 1;	//bottom left
					index[6] = i + tileList.width;		//bottom
					//index[7] = i + tileList.width + 1;	//bottom right
					if (tileList.adjacent[wid + hgt] >= 0)
					{
						SDL_BlitSurface(textures.textures[1], NULL, screen, &field.tiles[wid + hgt]);
					}
				}*/

				for (int c = 1; c <= 7; c++)	//when clicking on a tile adjecent to a bomb
				{
					if (tileList.adjacent[wid + hgt] == c)
					{
						SDL_BlitSurface(textures.textures[2 + c], NULL, screen, &field.tiles[wid + hgt]);
						break;
					}
				}
				if (tileList.bomb[wid + hgt] == 1) //clicking on a bomb
				{
					SDL_BlitSurface(textures.textures[2], NULL, screen, &field.tiles[wid + hgt]);
				}

			}

			SDL_UpdateWindowSurface(window);


		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
Tilelist gameStart(Difficulty d)
{
	Tilelist out;

	out.bomb = calloc(d.columns * d.rows, sizeof(_Bool));
	out.toggled = calloc(d.columns * d.rows, sizeof(_Bool));
	out.adjacent = calloc(d.columns * d.rows, sizeof(int));

	srand(clock(NULL));

	_Bool bombChance[100];
	//INT length = sizeof(bombChance) / sizeof(_Bool);
	for (INT i = 0; i <= d.bombPercent; i++)
	{
		bombChance[i] = 1; 
	}
	for (INT i = 99; i > d.bombPercent; i--)
	{
		bombChance[i] = 0;
	}
	for (INT i = 0; i < d.rows * d.columns; i++)
	{ 
		out.toggled[i] = 0;
		int r = rand() % 100;						//Should generate a number between 0 and 99,
		//SDL_Delay(r);								//which should select a random number in the boolean array.	
		out.bomb[i] = bombChance[r];				//Bomb Percent of 100 (i.e 60 0s and 40 1s)
													
		//out.bomb[i] = rand() % 2; //Should generate a number between 0 and 1 (true or false) (reference)
		//printf("%d", out.bomb[i]);
	}
	for (int debug = 0; debug < 100; debug++)
	{
		printf("%d", (int)bombChance[debug]);
	}
	for (int debug = 0; debug < d.rows * d.columns; debug++)
	{
		if (debug % d.columns == 0)
		{
			printf("\n");
		}
		printf("%d", (int)out.bomb[debug]);
	}
	for (INT i = 0; i < d.rows * d.columns; ++i)
	{
		//x = i % d.columns; // dont delete these, this is for reference
		//y = i / d.columns;

		int index[8];				// 3x3 with center missing
		index[0] = i - d.columns - 1;	//top left
		index[1] = i - d.columns;		//top
		index[2] = i - d.columns + 1;	//top right
		index[3] = i - 1;				//left
		index[4] = i + 1;				//right
		index[5] = i + d.columns - 1;	//bottom left
		index[6] = i + d.columns;		//bottom
		index[7] = i + d.columns + 1;	//bottom right
		
		if (out.bomb[i])
		{
			for (int n = 0; n < 8; n++)
			{
				if ((index[n] >= 0) && (index[n] < d.columns * d.rows))
				{
					if ((((i / d.columns) - (index[n] / d.columns) == 1)
						|| ((i / d.columns) - (index[n] / d.columns) == 0)
						|| ((i / d.columns) - (index[n] / d.columns) == -1))
						&& (((i % d.columns) - (index[n] % d.columns) == 1)
						|| ((i % d.columns) - (index[n] % d.columns) == 0)
						|| ((i % d.columns) - (index[n] % d.columns) == -1)))
					{
						out.adjacent[index[n]] += 1;
					}
				}
			}
		}
		//printf("%d", out.adjacent[i]);
	}
	printf("\n");
	for (int debug = 0; debug < d.rows * d.columns; debug++)
	{
		if (debug % d.columns == 0)
		{
			printf("\n");
		}
		printf("%d", (int)out.adjacent[debug]);
	}
	out.tileCount = d.columns * d.rows;
	out.height = d.rows;
	out.width = d.columns;

	return out;
}
Difficulty setDiff()
{
	/*printf("debug:setDiff\n");
	Difficulty d;
	char input[7];
	scanf("%s", input);
	if (!strcmp(input, "easy"))
	{
		d = easy;
	}
	if (!strcmp(input, "medium"))
	{
		d = medium;
	}
	if (!strcmp(input, "hard"))
	{
		d = hard;
	}
	else
	{
		printf("Enter bomb percent, between 0 and 100:");
		scanf("%f", &d.bombPercent);
		printf("columns:");
		scanf("%d", &d.columns);
		printf("rows:");
		scanf("%d", &d.rows);
	}*/
	Difficulty d;
	d = medium;
	return d;
}