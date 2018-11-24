#pragma once

// a structure to hold a particle
class Particle {
public:
	float color[3];
	float position[3];
	float direction[3];
	float life;
	Particle* next;
	Particle() {
		position[0] = 0.5f;
		position[2] = 0.5f; 
		position[1] = 0.0f;
		direction[0] = (10000 - rand() % 20000) / 10000.0f;
		direction[1] = (10000 - rand() % 20000) / 10000.0f;
		direction[2] = (10000 - rand() % 20000) / 10000.0f;
		life = rand() % 10000 / 10000.0f;
		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
	}
};

class ParticleSystem {
public:
	Particle * particle_head;

	// add
	void add() {
		Particle* p = new Particle();
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
			p->direction[1] += 9.81f * dt;
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

	// draw particles
	void drawParticles() {
		Particle* curr = particle_head;
		// glPointSize(2);
		// glBegin(GL_POINTS);
		// while (curr) {
		//   glVertex3fv(curr->position);
		//	 curr = curr->next;
		// }
		// glEnd();
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
};

ParticleSystem ps;

void updateParticles(float deltaTime) {
	if (areParticlesOn) {
		//Particles with box 1
		for (int i = 0; i < 50; i++) {
			ps.add();
		}
	}

	ps.update(deltaTime);
	ps.remove();
}