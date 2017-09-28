#pragma once

#include "resource.h"

#pragma once
class Sprite
{
public:
	int top, right, left, bottom, width, height;

	Sprite(int top, int left, int width, int height);

	void MoveLeft(int offset);
	void MoveUp(int offset);
	void MoveRight(int offset, int windowWidth);
	void MoveDown(int offset, int windowHeight);
};

