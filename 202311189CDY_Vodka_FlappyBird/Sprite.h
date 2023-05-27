
#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Vector.h"

typedef wchar_t TCHAR;


class Sprite
{
public:
	int fileIndex;
	DOUBLE_PAIR<int> size;
	DOUBLE_PAIR<int> hvFrames = { 1, 1 };
	int frame = 0;
};

#endif