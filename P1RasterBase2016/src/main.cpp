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


typedef struct {
	int minX, maxX, minY, maxY;
} Bbox;

typedef struct {
	float alpha, beta, gamma;
} Bcoords;

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

int w2pX(std::vector<tinyobj::shape_t> &shapes, int i, int coord, float left, float right, float bottom, float top, int old_width){
	float scale, shift;
	scale = (g_width - 1)/(right - left);
	shift = (-left)*((g_width-1)/(right-left));
	int val = scale*shapes[i].mesh.positions[coord]+shift;
	shapes[i].mesh.positions[coord] = val - (g_width - old_width)/2;
	return val;
	
}

int w2pY(std::vector<tinyobj::shape_t> &shapes, int i, int coord, float left, float right, float bottom, float top){
	float scale, shift;
	scale = (g_height - 1)/(top - bottom);
	shift = (-bottom)*((g_height - 1)/(top - bottom));
	int val = scale*shapes[i].mesh.positions[coord]+shift;
	shapes[i].mesh.positions[coord] = val;
	return val;
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

//	if (pxr > wr){
 //		tempw = w_width * g_height / w_height;
 //		temph = g_height;
   //      }
//         else {
//		tempw = g_width;
 //		temph = w_height * g_width / w_width;
 //       }
 	g_width = tempw;
 	g_height = temph;
}

void convertcoords(std::vector<tinyobj::shape_t> &shapes, vector<float> zbuff){
	// Variables
	float left, right, bottom, top, scale;
	int old_width = g_width, old_height = g_height, rlshift, tbshift, tempw, temph, tempX, tempY, minX = 2147483647, maxX = -2147483647, minY = 2147483647, maxY = -2147483647;

	getratio(shapes);
	
	if (g_width > g_height){
		left = -(g_width/g_height);
		right = (g_width/g_height);
		bottom = -1;
		top = 1;
	}
	else {
		left = -1;
		right = 1;
		bottom = -((g_height)/g_width);
		top = ((g_height)/g_width);
	}
	
	rlshift = (g_width - old_width) / 2;
	
	for (size_t i = 0; i < shapes.size(); i++){
		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++){
//			tempX = w2pX(shapes, i, 3*v+0, left, right, bottom, top);
//			tempY = w2pY(shapes, i, 3*v+1, left, right, bottom, top);
			tempX = w2pX(shapes, i, 3*v+0, left, right, bottom, top, old_width);
                        tempY = w2pY(shapes, i, 3*v+1, left, right, bottom, top);

			if (tempX < minX) minX = tempX;
			if (tempX > maxX) maxX = tempX;
			if (tempY < minY) minY = tempY;
			if (tempY > maxY) maxY = tempY;
			zbuff.push_back(shapes[i].mesh.positions[3*v+2]);
//			zbuff[3*v+0][3*v+1] = shapes[i].mesh.positions[3*v+2];
		}
	}

//cout << "minX = " << minX << " maxX = " << maxX << " minY = " << minY << " maxY = " << maxY << endl;
cout << "px_width = " << maxX-minX << " px_height = " << maxY-minY << endl;
cout << "g_width = " << g_width << " g_height = " << g_height << endl;
}

Triangle getTriangle(std::vector<tinyobj::shape_t> &shapes, int i, int v){
//	cout << "v = " << v << endl;

	Triangle t(0,0,0,0,0,0);
	int v1 = shapes[i].mesh.indices[3*v+0];
	t.v1x = shapes[i].mesh.positions[v1*3];
	t.v1y = shapes[i].mesh.positions[(v1*3)+1];

	int v2 = shapes[i].mesh.indices[3*v+1];
	t.v2x = shapes[i].mesh.positions[v2*3];
        t.v2y = shapes[i].mesh.positions[(v2*3)+1];

	int v3 = shapes[i].mesh.indices[3*v+2];
	t.v3x = shapes[i].mesh.positions[v3*3];
        t.v3y = shapes[i].mesh.positions[(v3*3)+1];

	t.minX = t.getMin(t.v1x, t.v2x, t.v3x);
	t.maxX = t.getMax(t.v1x, t.v2x, t.v3x);
	t.minY = t.getMin(t.v1y, t.v2y, t.v3y);
	t.maxY = t.getMax(t.v1y, t.v2y, t.v3y);

//	cout << "f " << v1+1 << " " << v2+1 << " " << v3+1 << endl;

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

int getabg(float triarea, Triangle t, int x, int y, Bcoords *bcoords){
	bcoords->beta = (((t.v1x-t.v3x)*(y-t.v3y))-((x-t.v3x)*(t.v1y-t.v3y)))/triarea;
        bcoords->gamma = (((t.v2x-t.v1x)*(y-t.v1y))-((x-t.v1x)*(t.v2y-t.v1y)))/triarea;
        bcoords->alpha = 1 - bcoords->beta - bcoords->gamma;

//	cout << "alpha = " << bcoords->alpha << " beta = " << bcoords->beta << " gamma = " << bcoords->gamma << endl;

	if ((bcoords->alpha >= 0 && bcoords->alpha <= 1) && (bcoords->beta >= 0 && bcoords->beta <= 1) && (bcoords->gamma >= 0 && bcoords->gamma <= 1)){
		return 1;
	}
	else {
		return 0;
	}
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
//        g_width = g_height = 100;    
	g_width = atoi(argv[3]);	// in px
	g_height = atoi(argv[4]);	// in px

	int c_mode = atoi(argv[5]);
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

//	resize_obj(shapes); 

	// ******** TODO add code to iterate through each triangle and rasterize it ********
	vector<float> zbuff;
//	float zbuff[g_width][g_height];
	convertcoords(shapes, zbuff);
	unsigned char r;
	unsigned char g;
	unsigned char b;
	float alpha, beta, gamma;
	Bcoords *bcoords = new Bcoords();


	for (size_t i = 0; i < shapes.size(); i++){
		for (size_t v = 0; v < shapes[i].mesh.indices.size() / 3; v++){
			Triangle t = getTriangle(shapes, i, v);
			float triarea = (((t.v2x-t.v1x)*(t.v3y-t.v1y))-((t.v3x-t.v1x)*(t.v2y-t.v1y)));
                        for (int y = t.minY; y <= t.maxY; ++y){
                                for (int x = t.minX; x <= t.maxX; ++x){
                                        beta = (((t.v1x-t.v3x)*(y-t.v3y))-((x-t.v3x)*(t.v1y-t.v3y)))/triarea;
                                        gamma = (((t.v2x-t.v1x)*(y-t.v1y))-((x-t.v1x)*(t.v2y-t.v1y)))/triarea;
        				alpha = 1 - beta - gamma;
					if ((alpha >= 0 && alpha <= 1) && (beta >= 0 && beta <= 1) && (gamma >= 0 && gamma <= 1)){
//						r = (alpha*148) + (beta*148) + (gamma*148);
//                                 		g = (alpha*215) + (beta*215) + (gamma*215);
//                                		b = (alpha*219) + (beta*219) + (gamma*219);
						image->setPixel(x,y,148,215,219);
					}	
//					else {
//						r = 0;
//						g = 0;
//						b = 0;
//					}
//					image->setPixel(x,y,r,g,b);					
				}
			}
		}
	}

	//write out the image
        image->writeToFile(imgName);


	return 0;
}
