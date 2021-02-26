#pragma once
#include "EntityMGR.h"

class SceneEntityMGRTemplate : public EntityMGR{
public:
	SceneEntityMGRTemplate();
	~SceneEntityMGRTemplate();

	virtual void Init();
	virtual void Update(double dt);
	virtual void ACAR(Entity* entity, float range, std::vector<Entity*> list);
	virtual void Interactions();
};