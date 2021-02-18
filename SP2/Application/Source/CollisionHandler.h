#pragma once
#include "Vector3.h"

class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	bool PointToAABB(Vector3 point, Vector3 min, Vector3 max);
	bool AABBtoAABB(Vector3 min1, Vector3 max1, Vector3 min2, Vector3 max2);
};

