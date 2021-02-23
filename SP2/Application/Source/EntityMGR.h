#pragma once
#include "Entity.h"
#include "CollisionHandler.h"
#include <vector>
#include <cstdlib>
#include <ctime>

class EntityMGR {
	CollisionHandler collExe;

public:
	Entity* temp;
	Entity* cardTemp;
	Entity under;
	std::vector<Entity*> entityList;
	std::vector<Entity*> nearbyList;
	std::vector<Entity*> emptyList;

	int interactionstate = 0;

	EntityMGR();
	~EntityMGR();
	virtual void Init();
	virtual void Update(double dt);
	std::string ACAR(Entity* entity, float range, std::vector<Entity*> list);

	void CreatePoint(std::string name, Vector3 pos);
	void CreateAABB(std::string name, Vector3 pos, Vector3 hMin, Vector3 hMax);
	void UpdateHitbox(Entity* entity);
	void UpdateListHitbox(std::vector<Entity*> list);
	void UpdateALLHitboxes();

	void registerEntity(Entity* entity);

	void checkNearby(Entity* entity, float range, std::vector<Entity*> list);
	void findEntity(std::string name);

	bool isCollided(Entity* entity1, Entity* entity2);
	
};