#pragma once
#include "Entity.h"
#include "Camera3.h"
#include "Vector3.h"

class Player : public Entity
{
public:
	Camera3* FPSCam;
	Vector3 tempPos;
	Vector3 tempTarget;
	Vector3 tempUp;

	Player(Vector3 pos, Vector3 target, Vector3 up, float radius);
	~Player();

	void Update(double dt);

	void updateTemp();
	void returnToTemp();
};

