#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {};
CollisionHandler::~CollisionHandler() {};

bool CollisionHandler::PointToAABB(Vector3 point, Vector3 min, Vector3 max) {
	return((point.x > min.x && point.x < max.x&&
			point.y > min.y && point.y < max.y&&
			point.z > min.z && point.z < max.z)); //this checks if the point is within the space of the 3D object.
}

bool CollisionHandler::AABBtoAABB(Vector3 min1, Vector3 max1, Vector3 min2, Vector3 max2) {
	return(min1.x < max2.x &&
		   max1.x > min2.x &&
		   min1.y < max2.y &&
		   max1.y > min2.y &&
		   min1.z < max2.z &&
		   max1.z > min2.z);
}
