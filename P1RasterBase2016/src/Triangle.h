#pragma once
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <string>
#include <vector>

class Triangle
{
public:
	Triangle(int v1x, int v1y, int v2x, int v2y, int v3x, int v3y);
	int getMin(int v1, int v2, int v3);
	int getMax(int v1, int v2, int v3);


	int v1x;
	int v1y;
	int v1zi;
	int v2x;
	int v2y;
	int v2zi;
	int v3x;
	int v3y;
	int v3zi;
	int minX;
	int maxX;
	int minY;
	int maxY;
};

#endif
