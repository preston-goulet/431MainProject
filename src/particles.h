#pragma once

#include "mesh.h"

static const char PARTICLE_LEAVES = 1;
static const char PARTICLE_RAIN = 2;

// a structure to hold a particle
class Particle {
public:
	Vec3f color;
	Vec3f position;
	Vec3f direction;
	float life;
	Particle* next;

	Particle() {
		color = Vec3f(1.0f, 1.0f, 1.0f);
		position = Vec3f(0.5f, 0.0f, 0.5f);
		direction = Vec3f((10000 - rand() % 20000) / 10000.0f,
			(10000 - rand() % 20000) / 10000.0f,
			(10000 - rand() % 20000) / 10000.0f);
		life = rand() % 1000 / 1000.0f;
	}

	Particle(Vec3f pos, Vec3f dir, Vec3f colors, float lifeLength) {
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		direction[0] = dir.x;
		direction[1] = dir.y;
		direction[2] = dir.z;
		life = lifeLength;
		color[0] = colors.x;
		color[1] = colors.y;
		color[2] = colors.z;
	}

	Particle* copy() {
		return new Particle(position, direction, color, life);
	}

	Particle* leaves() {
		return new Particle(Vec3f((1000 - (rand() % 2000)), 100.0f, (1000 - (rand() % 2000))),
			Vec3f((100 - (rand() % 200) / 100.0f), -100.0f, (100 - (rand() % 200) / 100.0f)),
			Vec3f(0.0f, 0.0f, 1.0f), 25);
	}

	Particle* rain() {
		return new Particle(Vec3f((1000 - (rand() % 2000)), 100.0f, (1000 - (rand() % 1000))),
			Vec3f(0, -500.0f, 0),
			Vec3f(0.0f, 0.0f, 0.0f), 10);
	}
};

class ParticleSystem {
public:
	
	Particle * particle_head = nullptr;
	Particle * reference;
	bool isThereGravity;
	GLuint display;
	int type;

	ParticleSystem() {
		isThereGravity = false;
		type = 0;
	}

	ParticleSystem(GLuint displayList, bool gravity, int typeVal) {
		isThereGravity = gravity;
		reference = new Particle();
		display = displayList;
		type = typeVal;
	}

	// add
	void add() {
		Particle* p = new Particle();
		p->next = particle_head;
		particle_head = p;
	}

	void addLeaves() {
		Particle* p = reference->leaves();
		p->next = particle_head;
		particle_head = p;
	}

	void addRain() {
		Particle* p = reference->rain();
		p->next = particle_head;
		particle_head = p;
	}

	// update
	void update(float dt) {
		Particle* p = particle_head;
		while (p) {
			// decrease lifespan
			p->life -= dt;
			// apply gravity
			if (isThereGravity) {
				p->direction[1] += -9.81f * dt;
			}
			
			// modify position
			p->position[0] += dt * p->direction[0];
			p->position[1] += dt * p->direction[1];
			p->position[2] += dt * p->direction[2];

			//Color
			p->color[1] -= .01;
			p->color[2] -= .02;

			// goto next particle
			p = p->next;
		}
	}

	// remove
	void remove() {
		Particle* curr = particle_head;
		Particle* prev = 0;
		while (curr) {
			if (curr->life<0) {
				if (prev) {
					prev->next = curr->next;
				}
				else {
					particle_head = curr->next;
				}
				Particle* temp = curr;
				curr = curr->next;
				delete temp;
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}
	}

	void draw() {
		Particle* curr = particle_head;
		if (!areParticlesOn) {
			return;
		}

		while (curr) {
			glPushMatrix();
			//glScalef(100.0, 100.0, 100.0);
			glTranslatef(curr->position[0], curr->position[1], curr->position[2]);
			glScalef(0.2, 0.2, 0.2);
			glColor4f(curr->color[0], curr->color[1], curr->color[2], 0.3);
			
			glCallList(display);

			glPopMatrix();
			curr = curr->next;
		}
	}

	// draw particles
	void drawDefaultParticles() {
		Particle* curr = particle_head;
		if (!areParticlesOn) {
			return;
		}

		while (curr) {
			glPushMatrix();
			glScalef(100.0, 100.0, 100.0);
			glTranslatef(curr->position[0], curr->position[1], curr->position[2]);
			glScalef(0.2, 0.2, 0.2);
			glColor4f(curr->color[0], curr->color[1], curr->color[2], 0.3);
			//front
			glBegin(GL_POLYGON); // fill connected polygon
			glVertex3f(0, 0, 0); // vertices of the triangle 1
			glVertex3f(0, 1, 0);
			glVertex3f(1, 1, 0);
			glVertex3f(1, 1, 0);
			glEnd();
			//back
			glBegin(GL_POLYGON); // fill connected polygon
			glVertex3f(0, 0, -1); // vertices of the triangle 1
			glVertex3f(0, 1, -1);
			glVertex3f(1, 1, -1);
			glVertex3f(1, 0, -1);
			glEnd();
			//left
			glBegin(GL_POLYGON); // fill connected polygon
			glVertex3f(0, 0, -1); // vertices of the triangle 1
			glVertex3f(0, 1, -1);
			glVertex3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glEnd();
			//right
			glBegin(GL_POLYGON); // fill connected polygon
			glVertex3f(1, 0, -1); // vertices of the triangle 1
			glVertex3f(1, 0, 0);
			glVertex3f(1, 1, 0);
			glVertex3f(1, 1, -1);
			glEnd();
			//top
			glBegin(GL_POLYGON); // fill connected polygon
			glVertex3f(1, 1, -1); // vertices of the triangle 1
			glVertex3f(1, 1, 0);
			glVertex3f(0, 1, 0);
			glVertex3f(0, 1, -1);
			glEnd();
			//bottom
			glBegin(GL_POLYGON); // fill connected polygon
			glVertex3f(1, 0, -1); // vertices of the triangle 1
			glVertex3f(1, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, -1);
			glEnd();

			glPopMatrix();
			curr = curr->next;
		}
	}

	void updateParticles(float deltaTime) {
		if (areParticlesOn) {
			//Particles with box 1
			switch (type) {
			case 0:
				for (int i = 0; i < 50; i++) {
					add();
				}
				break;

			case 1:
				addLeaves();
				break;

			case 2:
				addRain();
				break;
			}
		}

		update(deltaTime);
		remove();
	}
};

ParticleSystem ps;
ParticleSystem leaves;
ParticleSystem rain;