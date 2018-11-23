#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS

#include <GL\glut.h>
#include <windows.h>
#include <vector>
#include <imathvec.h>

using namespace std;
using namespace Imath;

class Texture {
public:
	static int textureCount;
	GLuint id;

	Texture();

	void loadBMP_custom(const char * imagepath);
};

#endif // !TEXTURE_H_INCLUDED