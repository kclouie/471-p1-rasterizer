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

void w2pX(std::vector<tinyobj::shape_t> &shapes, int i, int coord, float left, float right, float bottom, float top){
	cout << "wX = " << shapes[i].mesh.positions[coord] << endl;
	float scale, shift;
	scale = (g_width - 1)/(right - left);
	shift = (-left)*((g_width-1)/(right-left));
	int val = scale*shapes[i].mesh.positions[coord]+shift;
	shapes[i].mesh.positions[coord] = val;
	cout << "pX = " << shapes[i].mesh.positions[coord] << endl;
}

void w2pY(std::vector<tinyobj::shape_t> &shapes, int i, int coord, float left, float right, float bottom, float top){
	cout << "wY = " << shapes[i].mesh.positions[coord] << endl;
	float scale, shift;
	scale = (g_height - 1)/(top - bottom);
	shift = (-bottom)*((g_height - 1)/(top - bottom));
	int val = scale*shapes[i].mesh.positions[coord]+shift;
	shapes[i].mesh.positions[coord] = val;
        cout << "pY = " << shapes[i].mesh.positions[coord] << endl;
}

void rasterize(std::vector<tinyobj::shape_t> &shapes){
//	cout << "**** BEGIN RASTERIZE() ****" << endl;
	float left, right, bottom, top;

//      compute R,L,T,B
	if (g_width > g_height){
		left = -(g_width/g_height);
		right = (g_width/g_height);
		bottom = -1;
		top = 1;
	}
	else {
		left = -1;
		right = 1;
		bottom = -(g_height/g_width);
		top = (g_height/g_width);
	}

//	cout << "wX = " << shapes[0].mesh.positions[1] << endl;

//      for each vertex (xyz), use R,L,T,B to find mapped X and Y for each
	for (int i = 0; i < shapes.size(); i++){
		for (int v = 0; v < shapes[i].mesh.positions.size() / 3; v++){
			w2pX(shapes, i, 3*v+0, left, right, bottom, top);
			w2pY(shapes, i, 3*v+1, left, right, bottom, top);
		}
	}	

//      store Z

//      compute Bbox in px space
// ******** TODO insert Bbox struct ********

//      pull content from L02
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
//      g_width = g_height = 100;    *** if using this, change 'g_width = atoi...' and 'g_height = ...'
	g_width = atoi(argv[3]);
	g_height = atoi(argv[4]);
	
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
		posBuf = shapes[0].mesh.positions;
		triBuf = shapes[0].mesh.indices;
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;
	cout << "Number of triangles: " << triBuf.size()/3 << endl;

 	//keep this code to resize your object to be within -1 -> 1
	resize_obj(shapes); 

	// ******** TODO add code to iterate through each triangle and rasterize it ********
	rasterize(shapes);




	
	//write out the image
        image->writeToFile(imgName);

	return 0;
}
