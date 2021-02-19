#include "Player.h"

Player::Player(Vector3 pos, Vector3 target, Vector3 up, float radius) {
	FPSCam = new Camera3;
	FPSCam->Init(pos, target, up);
	this->pos = pos;
	this->name = "PlayerEntity";
	if (radius != 0) {
		dHitbox[0] = Vector3(radius, 0, radius); //dHitbox is the default hitbox of an entity and is automagically updated inside the EntityMGR if this entity is registered.
		dHitbox[1] = Vector3(pos.x + radius, pos.y + radius, pos.z + radius);
		this->isPoint = false;
	}
	else {
		this->isPoint = true;
	}
}

Player::~Player() {
	delete FPSCam;
}

void Player::Update(double dt) { //standard updating function.
	updateTemp();
	FPSCam->Update(dt);
	this->pos = FPSCam->position;
}

void Player::updateTemp() { //this holds a temporary set of info so that if a collision is detected, the camera can be reset so the camera doesnt glitch into structures.
	this->tempPos = FPSCam->position;
	this->tempTarget = FPSCam->target;
	this->tempUp = FPSCam->up;
}

void Player::returnToTemp() { //this resets all the info back to the temporary set.
	FPSCam->position = this->tempPos;
	FPSCam->target = this->tempTarget;
	FPSCam->up = this->tempUp;
	this->pos = this->tempPos;
}