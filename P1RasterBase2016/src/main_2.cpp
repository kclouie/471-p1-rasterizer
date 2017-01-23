/* Release code for program 1 CPE 471 Fall 2016 */

#include <iostream>
#include <string>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"
#include "Triangle.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int g_width, g_height;


/*
   Helper function you will want all quarter
   Given a vector of shapes which has already been read from an obj file
   resize all vertices to the range [-1, 1]
 */
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
   float minX, minY, minZ;
   float maxX, maxY, maxZ;
   float scaleX, scaleY, scaleZ;
   float shiftX, shiftY, shiftZ;
   float epsilon = 0.001;

   minX = minY = minZ = 1.1754E+38F;
   maxX = maxY = maxZ = -1.1754E+38F;

   //Go through all vertices to determine min and max of each dimension
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
         if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];

         if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
         if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];

         if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
         if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
      }
   }

   //From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
      maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
      maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
      maxExtent = zExtent;
   }
   scaleX = 2.0 /maxExtent;
   shiftX = minX + (xExtent/ 2.0);
   scaleY = 2.0 / maxExtent;
   shiftY = minY + (yExtent / 2.0);
   scaleZ = 2.0/ maxExtent;
   shiftZ = minZ + (zExtent)/2.0;

   //Go through all verticies shift and scale them
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
         assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
         assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
         assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);

      }
   }
}

void w2pX(std::vector<tinyobj::shape_t> &shapes, int i, int coord, float left, float right, float bottom, float top, int old_width){
	float scale, shift;
	scale = (g_width - 1)/(right - left);
	shift = (-left)*((g_width-1)/(right-left));
	int val = scale*shapes[i].mesh.positions[coord]+shift;
	shapes[i].mesh.positions[coord] = val - (g_width - old_width)/2;
}

void w2pY(std::vector<tinyobj::shape_t> &shapes, int i, int coord, float left, float right, float bottom, float top){
	float scale, shift;
	scale = (g_height - 1)/(top - bottom);
	shift = (-bottom)*((g_height - 1)/(top - bottom));
	int val = scale*shapes[i].mesh.positions[coord]+shift;
	shapes[i].mesh.positions[coord] = val;
}

void getratio(std::vector<tinyobj::shape_t> &shapes){
	float pxr, wr; //, xscale, yscale, scale;
	int w_width, w_height, tempw, temph;
	int minX = 2147483647, maxX = -2147483647, minY = 2147483647, maxY = -2147483647;
        for (size_t i = 0; i < shapes.size(); i++){
                for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++){
                        if ((minX > shapes[i].mesh.positions[3*v+0])) minX = shapes[i].mesh.positions[3*v+0];
                        if ((maxX < shapes[i].mesh.positions[3*v+0])) maxX = shapes[i].mesh.positions[3*v+0];
                        if ((minY > shapes[i].mesh.positions[3*v+1])) minY = shapes[i].mesh.positions[3*v+1];
                        if ((maxY < shapes[i].mesh.positions[3*v+1])) maxY = shapes[i].mesh.positions[3*v+1];
                }
        }
	w_width = (maxX-minX) + 1;
        w_height = (maxY - minY) + 1;
	wr = w_width / w_height;
	pxr = g_width / g_height;
	if (pxr > wr){
		tempw = g_width;
		temph = w_height * g_width / w_width;
        }
        else {
		tempw = w_width * g_height / w_height;
		temph = g_height;
        }
 	g_width = tempw;
 	g_height = temph;
}

void convertcoords(std::vector<tinyobj::shape_t> &shapes){
	float left, right, bottom, top, scale;
	int old_width = g_width, old_height = g_height;
	getratio(shapes);
	if (g_width > g_height){
		left = -g_width/g_height;
		right = g_width/g_height;
		bottom = -1;
		top = 1;
	}
	else {
		left = -1;
		right = 1;
		bottom = -g_height/g_width;
		top = g_height/g_width;
	}
	for (size_t i = 0; i < shapes.size(); i++){
		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++){
			w2pX(shapes, i, 3*v+0, left, right, bottom, top, old_width);
                        w2pY(shapes, i, 3*v+1, left, right, bottom, top);
		}
	}
}

Triangle getTriangle(std::vector<tinyobj::shape_t> &shapes, int i, int v){
	Triangle t(0,0,0,0,0,0);
	int v1 = shapes[i].mesh.indices[3*v+0];
	t.v1x = shapes[i].mesh.positions[v1*3];
	t.v1y = shapes[i].mesh.positions[(v1*3)+1];
	t.v1z = shapes[i].mesh.positions[(v1*3)+2];
	t.v1r = (255 - 0) / (1 + 1) * (t.v1z - 1) + 255;

	int v2 = shapes[i].mesh.indices[3*v+1];
	t.v2x = shapes[i].mesh.positions[v2*3];
        t.v2y = shapes[i].mesh.positions[(v2*3)+1];
	t.v2z = shapes[i].mesh.positions[(v2*3)+2];
        t.v2r = (255 - 0) / (1 + 1) * (t.v2z - 1) + 255;

	int v3 = shapes[i].mesh.indices[3*v+2];
	t.v3x = shapes[i].mesh.positions[v3*3];
        t.v3y = shapes[i].mesh.positions[(v3*3)+1];
	t.v3z = shapes[i].mesh.positions[(v3*3)+2];
        t.v3r = (255 - 0) / (1 + 1) * (t.v3z - 1) + 255;

	t.minX = t.getMin(t.v1x, t.v2x, t.v3x);
	t.maxX = t.getMax(t.v1x, t.v2x, t.v3x);
	t.minY = t.getMin(t.v1y, t.v2y, t.v3y);
	t.maxY = t.getMax(t.v1y, t.v2y, t.v3y);

	return t;
}

int getMin(int v1, int v2, int v3) {
        int min = v1;
        int arr[3] = {v1, v2, v3};
        for (int i = 0; i < 3; i++){
                if (arr[i] < min) min = arr[i];
        }
        return min;
}

int getMax(int v1, int v2, int v3) {
        int max = v1;
        int arr[3] = {v1, v2, v3};
        for (int i = 0; i < 3; i++){
                if (arr[i] > max) max = arr[i];
        }
        return max;
} 

int main(int argc, char **argv)
{
	if(argc < 3) {
		cout << "Usage: Assignment1 inputfile meshfile width height colormode" << endl;
    		return 0;
   	}
	// OBJ filename
	string meshName(argv[1]);
	string imgName(argv[2]);
	//set g_width and g_height appropriately!
	g_width = atoi(argv[3]);	
	g_height = atoi(argv[4]);	
	int mode = atoi(argv[5]);
        //create an image
	auto image = make_shared<Image>(g_width, g_height);
	// triangle buffer
	vector<unsigned int> triBuf;
	// position buffer
	vector<float> posBuf;
	// Some obj files contain material information.
	// We'll ignore them for this assignment.
	vector<tinyobj::shape_t> shapes; // geometry
	vector<tinyobj::material_t> objMaterials; // material
	string errStr;
        bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());
	/* error checking on read */
	if(!rc) {
		cerr << errStr << endl;
	} else {
		//keep this code to resize your object to be within -1 -> 1
		resize_obj(shapes);
		posBuf = shapes[0].mesh.positions;
		triBuf = shapes[0].mesh.indices;
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;
	cout << "Number of triangles: " << triBuf.size()/3 << endl;
	convertcoords(shapes);
	double zbuff[g_width][g_height];
	for (int s = 0; s < g_width; s++){
		for (int k = 0; k < g_height; k++){
			zbuff[s][k] = -std::numeric_limits<double>::infinity();
		}
	}
	unsigned char r;
	unsigned char g;
	unsigned char b;
	float alpha, beta, gamma;

	for (size_t i = 0; i < shapes.size(); i++){
		for (size_t v = 0; v < shapes[i].mesh.indices.size() / 3; v++){
			Triangle t = getTriangle(shapes, i, v);
			float triarea = (((t.v2x-t.v1x)*(t.v3y-t.v1y))-((t.v3x-t.v1x)*(t.v2y-t.v1y)));
                        for (int y = t.minY; y <= t.maxY; ++y){
                                for (int x = t.minX; x <= t.maxX; ++x){
                                        beta = (((t.v1x-t.v3x)*(y-t.v3y))-((x-t.v3x)*(t.v1y-t.v3y)))/triarea;
                                        gamma = (((t.v2x-t.v1x)*(y-t.v1y))-((x-t.v1x)*(t.v2y-t.v1y)))/triarea;
        				alpha = 1 - beta - gamma;
					if (((alpha >= 0 && alpha <= 1) && (beta >= 0 && beta <= 1) && (gamma >= 0 && gamma <= 1))){
						if (mode == 1){
							double currz = alpha*t.v1z + beta*t.v2z + gamma*t.v3z;
                                                        if (currz > zbuff[x][y]){
                                                                r = 50;
                                                                g = 0;
                                                                b = ((currz + 1) / (1 + 1)) * (255 - 0) + 0;
                                                                zbuff[x][y] = currz;
                                                                image->setPixel(x,y,r,g,b);
                                                        }
                                                }
						if (mode == 2){
							double curry = alpha*t.v1y + beta*t.v2y + gamma*t.v3y;
							r = 2;
							g = 10;
							b = ((curry + 1) / (1 + 1)) * (255 - 0) + 0;
							image->setPixel(x,y,r,g,b);
						}
					}	
				}
			}
		}
	}

	//write out the image
        image->writeToFile(imgName);

	return 0;
}
