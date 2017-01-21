#include <iostream>
#include <cassert>
#include "Triangle.h"

using namespace std;

Triangle::Triangle(int v1x, int v1y, int v2x, int v2y, int v3x, int v3y) {
	v1x = v1x;
	v1y = v1y;
	v2x = v2x;
	v2y = v2y;
	v3x = v3y;
	v3y = v3y;
	minX = 0;
	maxX = 0;
	minY = 0;
	maxY = 0;
}

int Triangle::getMinX(int v1x, int v2x, int v3x) {
	int min = 0;
	if (v1x <= v2x && v1x <= v3x) {
		min = v1x;
	}
	else if (v2x <= v1x && v2x <= v3x) {
		min = v2x;
	}	
	else {
		min = v3x;
	}
	return min;
}

int Triangle::getMaxX(int v1x, int v2x, int v3x) {
	int max = 0;
	if (v1x >= v2x && v1x >= v3x) {
		max = v1x;
	}
	else if (v2x >= v1x && v2x >= v3x) {
		max = v2x;
	}
	else {
		max = v3x;
	}
	return max;
}

int Triangle::getMinY(int v1y, int v2y, int v3y) {
	int min = 0;
	if (v1y <= v2y && v1y <= v3y) {
		min = v1y;
	}
	else if (v2y <= v1y && v2y <= v3y) {
		min = v2y; 
	}
	else {
		min = v3y;
	}
	return min;
}

int Triangle::getMaxY(int v1y, int v2y, int v3y) {
	int max = 0;
	if (v1y >= v2y && v1y >= v3y) {
		max = v1y;
	}
	else if (v2y >= v1y && v2y >= v3y) {
		max = v2y;
	}
	else {
		max = v3y;
	}
	return max;
}
