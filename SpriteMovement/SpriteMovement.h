#pragma once

#include "resource.h"

#pragma once
class Rectanglie
{
public:
	int top, right, left, bottom;

	Rectanglie(int top, int left, int width, int height);

	void MoveLeft(int offset);
	void MoveUp(int offset);
	void MoveRight(int offset, int windowWidth);
	void MoveDown(int offset, int windowHeight);
};

