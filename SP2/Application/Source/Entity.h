#pragma once
#include "Vector3.h"
#include <iostream>

struct Entity {
	Vector3 pos;
	Vector3 tempPos;
	Vector3 dHitbox[2];
	Vector3 uHitbox[2];

	std::string name;

	bool isPoint;
	bool isMoving = false;
	bool isCollided = false;
};