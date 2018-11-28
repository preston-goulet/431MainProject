#pragma once

#include <Imath/imathvec.h>
#include <vector>

using namespace Imath;
using namespace std;


typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;

class Curve {
public:
	int numberPoints;
	Vec3f start;
	Vec3f stop;
	Vec3f tanStart;
	Vec3f tanStop;
	vector<Vec3f> points;
	Curve() {
		start = Vec3f(0.0f, 0.0f, 0.0f);
		stop = Vec3f(0.0f, 0.0f, 0.0f);
		tanStart = Vec3f(0.0f, 0.0f, 0.0f);
		tanStop = Vec3f(0.0f, 0.0f, 0.0f);
		numberPoints = 128;
	}
	Curve(Vec3f startVec3f, Vec3f stopVec3f, Vec3f tanStartVec3f, Vec3f tanStopVec3f, int numPoints) {
		start = startVec3f;
		stop = stopVec3f;
		tanStart = tanStartVec3f;
		tanStop = tanStopVec3f;
		numberPoints = numPoints;
	}

	vector<Vec3f> calculatePoints();
	void drawLines();
	void drawPoints();
	Vec3f getPoint(int index);
};

vector<Vec3f> Curve::calculatePoints() {
	// use the parametric time value 0 to 1
	for (int i = 0; i != numberPoints; ++i) {
		float t = (float)i / (numberPoints - 1);
		// calculate blending functions
		float b0 = 2 * t*t*t - 3 * t*t + 1;
		float b1 = -2 * t*t*t + 3 * t*t;
		float b2 = t * t*t - 2 * t*t + t;
		float b3 = t * t*t - t * t;
		// calculate the x, y and z of the curve point
		float x = b0 * start.x + b1 * stop.x + b2 * tanStart.x + b3 * tanStop.x;
		float y = b0 * start.y + b1 * stop.y + b2 * tanStart.y + b3 * tanStop.y;
		float z = b0 * start.z + b1 * stop.z + b2 * tanStart.z + b3 * tanStop.z;
		// specify the point
		points.push_back(Vec3f(x, y, z));
	}

	return points;
}

Vec3f Curve::getPoint(int index) {
	if (index >= points.size()) {
		index = numberPoints - 1;
	}
	return points.at(index);
}

void Curve::drawLines() {
	glBegin(GL_LINE_STRIP);
	for (int i = 1; i < points.size() ; ++i) {	

		Vec3f point = getPoint(i);
		glVertex3f(point.x, point.y, point.z);		
	}
	glEnd();
}

void Curve::drawPoints() {
	glBegin(GL_POINTS);
	for (int i = 0; i != points.size(); ++i)
	{
		Vec3f point = getPoint(i);
		glVertex3f(point.x, point.y, point.z);
	}
	glEnd();
}

Curve jetCurveStart = Curve(Vec3f(-100, 0, -100),
	Vec3f(100, 0, 100),	
	Vec3f(100, 0, 0),
	Vec3f(-100, 0, 0), 500
);

Curve jetCurveEnd = Curve(Vec3f(100, 0, 100),
	Vec3f(-100, 0, -100),
	Vec3f(-100, 0, 0),
	Vec3f(100, 0, 0), 500
);