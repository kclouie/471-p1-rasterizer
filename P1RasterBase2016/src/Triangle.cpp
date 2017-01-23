#include <iostream>
#include <cassert>
#include "Triangle.h"

using namespace std;

Triangle::Triangle(int v1x, int v1y, int v2x, int v2y, int v3x, int v3y) {
	v1x = v1x;
	v1y = v1y;
	v1z = 0.0;
	v1r = 0;
	v1g = 0;
	v1b = 0;
	v2x = v2x;
	v2y = v2y;
	v2z = 0.0;
	v2r = 0;
	v2g = 0;
	v2b = 0;
	v3x = v3y;
	v3y = v3y;
	v3z = 0.0;
	v3r = 0;
	v3g = 0;
	v3b = 0;
	minX = 0;
	maxX = 0;
	minY = 0;
	maxY = 0;
}

int Triangle::getMin(int v1, int v2, int v3) {
        int min = v1;
        int arr[3] = {v1, v2, v3};
        for (int i = 0; i < 3; i++){
                if (arr[i] < min) min = arr[i];
        }
        return min;
}

int Triangle::getMax(int v1, int v2, int v3) {
        int max = v1;
        int arr[3] = {v1, v2, v3};
        for (int i = 0; i < 3; i++){
                if (arr[i] > max) max = arr[i];
        }
        return max;
}

