#include "GameObjectCollection.h"

void GameObjectCollection::pushToList(GameObject* gameobject) {

}

GameObject* GameObjectCollection::getAt(int index) {
	return &gameObjects.at(index);
}

void GameObjectCollection::toDisplayList() {
	for each(GameObject x in gameObjects) {
		x.toDisplayList();
	}
}
