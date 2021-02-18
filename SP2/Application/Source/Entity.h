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
	bool isUp = false;
	bool isCollided = false;
	
	float angle;
	float Ay = 0; //acceleration along the y axis.
	float Ny = 0; //force along the y axis.
	float kg = 1; //weight of object.
};