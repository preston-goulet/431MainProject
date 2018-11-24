#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Mesh.h"
#include "Texture.h"

class GameObject {
private:
	GLuint display;	
	Vec3f* position;
	Vec3f* rotation;

public:
	Mesh mesh;
	Texture texture;
	bool isActive;

	GameObject();

	void pushToList();

	//Getters & Setters
	void setPosition(Vec3f* position);
	void setRotation(Vec3f* rotation);
	void setDisplay(GLuint display);

	Vec3f* getPosition();
	Vec3f* getRotation();
	GLuint getDisplay();

	void meshToDisplayList(int id);
	void toDisplayList();
};

#endif // !GAMEOBJECT_H_INCLUDED

