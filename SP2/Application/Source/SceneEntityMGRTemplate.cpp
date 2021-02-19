#include "SceneEntityMGRTemplate.h"
#include "Application.h"
#include <sstream>

SceneEntityMGRTemplate::SceneEntityMGRTemplate() {
	
};
SceneEntityMGRTemplate::~SceneEntityMGRTemplate() {};

void SceneEntityMGRTemplate::Init() {
	//initialize things you need for your scene here
}

void SceneEntityMGRTemplate::Update(double dt) {
	//update things you need for your scene here
	UpdateALLHitboxes();
	PlayerEntity->Update(dt);
	ACAR(PlayerEntity, 5, entityList);
	//probably want to add interactions function in here when you sort out what you want the interactions to be like.
}

void SceneEntityMGRTemplate::ACAR(Entity* entity, float range, std::vector<Entity*> list) {
	checkNearby(entity, range, list);
	for (int i = 0; nearbyList.size(); i++) {
		if (isCollided(entity, nearbyList[i])) {
			if (nearbyList[i]->name == "change this to the name of the object") {//fill this part with your interactions if they are based on collisions.
				//do something
			}
			if (entity == this->PlayerEntity) {
				this->PlayerEntity->returnToTemp(); // this is necessary so the player doesnt walk through walls.
			}
		}
	}
}

void SceneEntityMGRTemplate::Interactions() {
	//this entirely depends on what types of interactions you intend on implementing :P
}