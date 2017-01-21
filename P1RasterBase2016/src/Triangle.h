#pragma once
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <string>
#include <vector>

class Triangle
{
public:
	Triangle(int v1x, int v1y, int v2x, int v2y, int v3x, int v3y);
	int getMinX(int v1x, int v2x, int v3x);
	int getMaxX(int v1x, int v2x, int v3x);
	int getMinY(int v1y, int v2y, int v3y);
	int getMaxY(int v1y, int v2y, int v3y);


	int v1x;
	int v1y;
	int v2x;
	int v2y;
	int v3x;
	int v3y;
	int minX;
	int maxX;
	int minY;
	int maxY;
};

#endif
