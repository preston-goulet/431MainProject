#include "GameObject.h"

GameObject::GameObject() {
	texture = Texture();
	isActive = true;
	position = new Vec3f(0, 0, 0);
	rotation = new Vec3f(0, 0, 0);
}

void GameObject::setPosition(Vec3f* position) {
	this->position = position;
}

void GameObject::setRotation(Vec3f* rotation) {
	this->rotation = rotation;
}

void GameObject::setDisplay(GLuint display) {
	this->display = display;
}

Vec3f* GameObject::getPosition() {
	return position;
}

Vec3f* GameObject::getRotation() {
	return rotation;
}

GLuint GameObject::getDisplay() {
	return display;
}

// draw
void GameObject::meshToDisplayList(int id) {
	display = glGenLists(id);
	glNewList(display, GL_COMPILE);
	//if (id != 3) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	//}
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < mesh.face_index_vertex.size(); i++) {
		// PER VERTEX NORMALS
		if ((!mesh.dot_normalPerVertex.empty() && !mesh.face_index_normalPerVertex.empty())) {
			glNormal3fv(&mesh.dot_normalPerVertex[mesh.face_index_normalPerVertex[i]].x);
		}
		// TEXTURES
		if (!mesh.dot_texture.empty() && !mesh.face_index_texture.empty()) {
			glTexCoord2fv(&mesh.dot_texture[mesh.face_index_texture[i]].x);
		}
		// COLOR
		Vec3f offset = (mesh.dot_vertex[mesh.face_index_vertex[i]]);
		// VERTEX
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&mesh.dot_vertex[mesh.face_index_vertex[i]].x);
	}
	glEnd();
	if (id != 3) {
		glDisable(GL_TEXTURE_2D);
	}
	glEndList();
}

void GameObject::toDisplayList() {
	if (isActive) {
		glPushMatrix();
		
		glTranslatef(position->x, position->y, position->z);
		glRotatef(rotation->x, 1.0, 0.0, 0.0);
		glRotatef(rotation->y, 0.0, 1.0, 0.0);
		glRotatef(rotation->z, 0.0, 0.0, 1.0);
		glCallList(display);

		glPopMatrix();
	}
}
