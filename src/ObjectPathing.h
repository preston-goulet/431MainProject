#include "CurveFollower.h"

class ObjectPathing {
private:
    CurveFollower* follower;

public:
    GameObject* gameObject;
    Curve* start;
    Curve* end;
    bool isSwapped;

    ObjectPathing() {
        gameObject = new GameObject();
        start = new Curve();
        end = new Curve();
        isSwapped = false;
        follower = new CurveFollower(start);
    }

    ObjectPathing(Curve* curveStart, Curve* curveEnd, GameObject* gameObj) {
          gameObject = gameObj;
          start = curveStart;
          end = curveEnd;
          isSwapped = false;
          follower = new CurveFollower(curveStart);
    }

    void update() {
        Vec3f point = follower->setNextPoint();
        if (follower->indexAtEnd()) {
            swapPaths();
            
        }
        //printf("%d %d %d \n", point.x, point.y, point.z);

        gameObject->position[0] = point.x; 
        gameObject->position[1] = point.y;
        gameObject->position[2] = point.z;
    }

    void updateRotation() {
        Vec3f pos = Vec3f(gameObject->position[0], gameObject->position[1], gameObject->position[2]);
        Vec3f next = follower->getNextPoint();
        Vec3f up = Vec3f(0, 1, 0).normalize();

        Vec3f z = (pos - next).normalize();
        Vec3f x = (z.cross(up)).normalize();
        Vec3f y = x.cross(z);

        z.negate();

        gameObject->rotation[0] = -x.dot(next);
        gameObject->rotation[1] = -y.dot(next);
        gameObject->rotation[2] = -z.dot(next);
    }

    void swapPaths() {
        if(isSwapped) {
            follower = new CurveFollower(start);
        } else {
            follower = new CurveFollower(end);
        }
        isSwapped = !isSwapped;
    }

    void drawPath() {
        if(areCurveLinesOn) {
            start->drawLines();
            end->drawLines();
        }

        if(areCurvePointsOn) {
            start->drawPoints();
            end->drawPoints();
        }
    }
};

ObjectPathing jetPath = ObjectPathing(&jetCurveStart, &jetCurveEnd, &jet);
float pathLocation[3] = {0, 1000, playArea};