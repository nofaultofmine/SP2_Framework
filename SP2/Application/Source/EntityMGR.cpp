#include "EntityMGR.h"
#include "Application.h"
#include <sstream>

EntityMGR::EntityMGR() {
	
};
EntityMGR::~EntityMGR() {};

void EntityMGR::Init() {
	
}

void EntityMGR::CreatePoint(std::string name, Vector3 pos) {
	temp = new Entity;
	temp->pos = pos;
	temp->name = name;
	temp->isPoint = true;
	registerEntity(temp);
}

void EntityMGR::CreateAABB(std::string name, Vector3 pos, Vector3 hMin, Vector3 hMax) {
	temp = new Entity;
	temp->pos = pos;
	temp->name = name;
	temp->isPoint = false;
	temp->dHitbox[0] = hMin;
	temp->dHitbox[1] = hMax;
	registerEntity(temp);
}

void EntityMGR::registerEntity(Entity* entity) {
	entityList.push_back(entity);
	std::cout << entity->name << " Registered in Entity List." << std::endl;
}

void EntityMGR::Update(double dt) {
	UpdateALLHitboxes();
}

bool EntityMGR::isCollided(Entity* entity1, Entity* entity2) {
	bool collided = false;
	if (entity1->isPoint && !entity2->isPoint) {
		collided = collExe.PointToAABB(entity1->pos, entity2->uHitbox[0], entity2->uHitbox[1]);
	}
	else if (!entity1->isPoint && entity2->isPoint) {
		collided = collExe.PointToAABB(entity2->pos, entity1->uHitbox[0], entity1->uHitbox[1]);
	}
	else if (!entity1->isPoint && !entity2->isPoint) {
		collided = collExe.AABBtoAABB(entity1->uHitbox[0], entity1->uHitbox[1], entity2->uHitbox[0], entity2->uHitbox[1]);
	}
	return collided;
}

void EntityMGR::checkNearby(Entity* entity, float range, std::vector<Entity*> list) {
	nearbyList = emptyList;
	float lengthchecker = 0;
	for (int i = 0; i < list.size(); i++) {
		lengthchecker = sqrt((entity->pos.x - list[i]->pos.x) * (entity->pos.x - list[i]->pos.x) + (entity->pos.z - list[i]->pos.z) * (entity->pos.z - list[i]->pos.z));
		if (lengthchecker < range) {
			nearbyList.push_back(list[i]);
			
		}
	}
}

void EntityMGR::findEntity(std::string name) {
	for (int i = 0; i < entityList.size(); i++) {
		if (entityList[i]->name == name){
			temp = entityList[i];
		}
	}
}

void EntityMGR::UpdateHitbox(Entity* entity) {
	for (int i = 0; i < 2; i++) {
		entity->uHitbox[i] = entity->pos + entity->dHitbox[i];
	}
}
void EntityMGR::UpdateListHitbox(std::vector<Entity*> list) {
	for (int i = 0; i < list.size(); i++) {
		UpdateHitbox(list[i]);
	}
}
void EntityMGR::UpdateALLHitboxes() {
	for (int i = 0; i < entityList.size(); i++) {
		UpdateHitbox(entityList[i]);
	}
}
