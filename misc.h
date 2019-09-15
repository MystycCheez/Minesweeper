#ifndef _misc_h_
#define _misc_h_

#include <Windows.h>

typedef struct Tilelist
{
	_Bool *bomb, *toggled;
	int *adjacent;
	int tileCount;
	int width;
	int height;
} Tilelist;
typedef struct Difficulty
{
	INT columns;
	INT rows;
	INT bombPercent;
} Difficulty;
typedef struct tempF
{
	INT x;
} tempF;

#endif