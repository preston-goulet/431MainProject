#pragma once

#include <Imath/imathvec.h>
#include <vector>

using namespace Imath;
using namespace std;


typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;

/*
//Top
Vec3f start = Vec3f(right_x, right_y, right_z);
Vec3f stop = Vec3f(left_x + 100, left_y, left_z);
Vec3f tan1 = Vec3f(-250, 0, -500);
Vec3f tan2 = Vec3f(-250, 0, -500);

vector<Vec3f> top = calculatePoints(&start, &stop, &tan1, &tan2);

//Bottom
start = Vec3f(right_x, right_y - 100, right_z);
stop = Vec3f(left_x + 100, left_y - 100, left_z);

vector<Vec3f> bottom = calculatePoints(&start, &stop, &tan1, &tan2);

makeMeshFrom2Lines(&top, &bottom);
*/

class Curve {
public:
	int N = 128;
	Vec3f* start;
	Vec3f* stop;
	Vec3f* tanStart;
	Vec3f* tanStop;
	vector<Vec3f> points;

	Curve(Vec3f* startVec3f, Vec3f* stopVec3f, Vec3f* tanStartVec3f, Vec3f* tanStopVec3f, int numPoints) {
		start = startVec3f;
		stop = stopVec3f;
		tanStart = tanStartVec3f;
		tanStop = tanStopVec3f;
		N = numPoints;
	}
	vector<Vec3f> calculatePoints();
};

vector<Vec3f> Curve::calculatePoints() {
	vector<Vec3f> points;

	// use the parametric time value 0 to 1
	for (int i = 0; i != N; ++i) {
		float t = (float)i / (N - 1);
		// calculate blending functions
		float b0 = 2 * t*t*t - 3 * t*t + 1;
		float b1 = -2 * t*t*t + 3 * t*t;
		float b2 = t * t*t - 2 * t*t + t;
		float b3 = t * t*t - t * t;
		// calculate the x, y and z of the curve point
		float x = b0 * start->x + b1 * stop->x + b2 * tan1->x + b3 * tan2->x;
		float y = b0 * start->y + b1 * stop->y + b2 * tan1->y + b3 * tan2->y;
		float z = b0 * start->z + b1 * stop->z + b2 * tan1->z + b3 * tan2->z;
		// specify the point
		
		points.push_back(Vec3f(x, y, z));
	}

	return points;
}

Curve jetCurve;
Curve bulletCurve;