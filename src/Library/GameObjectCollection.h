#ifndef GAMEOBJECTCOLLECTION_H_INCLUDED
#define GAMEOBJECTCOLLECTION_H_INCLUDED

#include "GameObject.h"
#include <vector>

class GameObjectCollection {
public:
	vector<GameObject> gameObjects;

	void pushToList(GameObject* gameobject);
	GameObject* getAt(int index);
	void toDisplayList();
};

#endif // !GAMEOBJECTCOLLECTION_H_INCLUDED

