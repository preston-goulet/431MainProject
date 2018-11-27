#pragma once
#include "Curve.h"

class CurveFollower {
public:
	Curve* curve;
	int index = 0;
	CurveFollower() {
		curve = new Curve();
	}
	CurveFollower(Curve* c) {
		curve = c;
		curve->points = curve->calculatePoints();
	}
	Vec3f getCurrentPoint();
	Vec3f getNextPoint();
	Vec3f getPrevPoint();
	Vec3f setNextPoint();
	Vec3f setPrevPoint();
	Vec3f setIndex(int curveIndex);
	bool indexAtEnd();
	
};

Vec3f CurveFollower::getCurrentPoint() {
	if (index >= curve->numberPoints) {
		index = curve->numberPoints - 1;
	}
	return curve->points.at(index);
}

Vec3f CurveFollower::getNextPoint() {
	int len = curve->points.size();
	if (index < len) {
		return curve->points.at(index + 1);
	}
	return getCurrentPoint();
}

Vec3f CurveFollower::getPrevPoint() {
	if (index > 0) {
		return curve->points.at(index - 1);
	}
	return getCurrentPoint();
}

Vec3f CurveFollower::setNextPoint() {
	int len = curve->points.size();
	if (index < len) {
		index++;
	}
	return getCurrentPoint();
}

Vec3f CurveFollower::setPrevPoint() {
	if (index > 0) {
		index--;
	}
	return getCurrentPoint();
}

Vec3f CurveFollower::setIndex(int curveIndex) {
	int length = curve->points.size() -1;
	if (curveIndex >= 0 && curveIndex < length) {
		index = curveIndex;
	}
	return getCurrentPoint();
}

bool CurveFollower::indexAtEnd() {
	return index == curve->numberPoints;
}

CurveFollower jetFollower = CurveFollower(&jetCurveStart);
