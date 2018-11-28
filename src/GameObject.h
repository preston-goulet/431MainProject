#pragma once

#define PI 3.1415927
#define DEG_TO_RAD (PI/ 180)

class GameObject {
public:
	bool updatePosition;
	float position[3];
	float rotation[3];
	float direction[3];
	int life = 0;
	float velocity[3];
	float acceleration[3];
	
	GameObject() {
		position[0] = 0.0f;
		position[2] = 0.0f;
		position[1] = 0.0f;
		direction[0] = 0.0f;
		direction[1] = 0.0f;
		direction[2] = 0.0f;
	}

	GameObject(float posX, float posY, float posZ, 
		float velX, float velY, float velZ, 
		float accX, float accY, float accZ,
		float rotX, float rotY, float rotZ, 
		bool haveUpdate) {
		position[0] = posX;
		position[1] = posY;
		position[2] = posZ;
		velocity[0] = velX;
		velocity[1] = velY;
		velocity[2] = velZ;
		acceleration[0] = accX;
		acceleration[1] = accY;
		acceleration[2] = accZ;
		rotation[0] = rotX;
		rotation[1] = rotY;
		rotation[2] = rotZ;
		updatePosition = haveUpdate;
	}

	GameObject(Vec3f pos, Vec3f vel, Vec3f acc, Vec3f rot, bool haveUpdate) {
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		velocity[0] = vel.x;
		velocity[1] = vel.y;
		velocity[2] = vel.z;
		acceleration[0] = acc.x;
		acceleration[1] = acc.y;
		acceleration[2] = acc.z;
		rotation[0] = rot.x;
		rotation[1] = rot.y;
		rotation[2] = rot.z;
		updatePosition = haveUpdate;	 
	}

	void update(float dt) {

		if(!updatePosition) {
			return;
		}

		direction[0] = cos(rotation[0] * DEG_TO_RAD) * cos(rotation[1] * DEG_TO_RAD);
		direction[1] = cos(rotation[0] * DEG_TO_RAD) * sin(rotation[1] * DEG_TO_RAD);
		direction[2] = sin(rotation[0] * DEG_TO_RAD);

		velocity[0] += (1 / 2) * acceleration[0] * (dt*dt) * direction[0];
		velocity[1] += (1 / 2) * acceleration[1] * (dt*dt) * direction[1];
		velocity[2] += (1 / 2) * acceleration[2] * (dt*dt) * direction[2];
		
		position[0] += dt * velocity[0]  + dt * velocity[0] * direction[0];
		position[1] += dt * velocity[1]  + dt * velocity[1] * direction[1];
		position[2] += dt * velocity[2]  + dt * velocity[2] * direction[2];
		
	}

	void setDirection(float x, float y, float z) {
		direction[0] = x;
		direction[1] = y;
		direction[2] = z;
	}

	void setPostion(float x, float y, float z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}

	void addToPostion(float x, float y, float z) {
		position[0] += x;
		position[1] += y;
		position[2] += z;
	}

	void calculateRotation(Vec3f vector) {

		vector.normalize();
		rotation[0] = 1 / tan(vector.y / vector.x);
		rotation[1] = 1 / tan(vector.z / vector.x);
		rotation[2] = 1 / tan(vector.y / vector.z);
	}
};

GameObject jet = GameObject(
	0.0f, 300.0f, 0.0f,
	0.0f, 0.0f, -100.0f,
	0.0f, 0.0f, 10.0f,
	90.0f, 180.0f, 0.0f,
	false);

void updateGameObjects(float deltaTime) {	
	jet.update(deltaTime);
}