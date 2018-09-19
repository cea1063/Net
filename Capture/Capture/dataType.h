#pragma once
#include <stdio.h>

typedef struct {
	int x;
	int y;
}Position;

typedef struct {
	Position start;
	int height;
	int width;
}RectInfo;