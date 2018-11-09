#pragma once

class Object {
public:
	float position[3];
	float direction[3];
	int life = 0;
	float velocity[3];
	float acceleration[3];
	
	Object() {
		position[0] = 0.0f;
		position[2] = 0.0f;
		position[1] = 0.0f;
		direction[0] = 0.0f;
		direction[1] = 0.0f;
		direction[2] = 1.0f;
	}
	Object(float posX, float posY, float posZ, float dirX, float dirY, float dirZ) {
		position[0] = posX;
		position[1] = posY;
		position[2] = posZ;
		direction[0] = dirX;
		direction[1] = dirY;
		direction[2] = dirZ;
	}

	void update(float dt);
	void setDirection(float x, float y, float z);
	void setPostion(float x, float y, float z);
	void addToPostion(float x, float y, float z);
	
};

void Object::update(float dt) {
	direction[0] = (1 / 2) * acceleration[0] * (dt*dt);
	direction[1] = (1 / 2) * acceleration[1] * (dt*dt);
	direction[2] = (1 / 2) * acceleration[2] * (dt*dt);
	position[0] = dt * direction[0];
	position[1] = dt * direction[1];
	position[2] = dt * direction[2];
}

void Object::setDirection(float x, float y, float z) {
	direction[0] = x;
	direction[1] = y;
	direction[2] = z;
}

void Object::setPostion(float x, float y, float z) {
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void Object::addToPostion(float x, float y, float z) {
	position[0] += x;
	position[1] += y;
	position[2] += z;
}