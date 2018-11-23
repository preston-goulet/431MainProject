

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <glut.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <Imath/imathvec.h>
#include <iostream>
#include<string>
#include <wingdi.h>
#include <GL\glut.h>

using namespace std;
using namespace Imath;

typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;
typedef float GLfloat;

class Mesh {
public:
	
	vector<Vec3f> dot_vertex;
	vector<Vec3f> dot_normalPerFace;
	vector<Vec3f> dot_normalPerVertex;
	vector<Vec2f> dot_texture;
	// faces
	vector<int> face_index_vertex;
	vector<int> face_index_normalPerFace;
	vector<int> face_index_normalPerVertex;
	vector<int> face_index_texture;

	int StrToInt(const string & str);
	vector<string> split_string(const string & str, const string & split_str);
	void loadFile(const char * file);
	void createCube();
	void createSkybox(int w);
	void createPlane(int arena_width, int arena_depth, int arena_cell);
	float** createPerlinPlane(int arena_width, int arena_depth, int arena_cell);
	void calculateNormalPerFace();
	void calculateNormalPerVertex();

};

#endif // !MESH_H_INCLUDED