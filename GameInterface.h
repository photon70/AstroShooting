#pragma once

#include "Header.h"

struct Player;
struct Bullet;
struct BulletContainer;
struct BulletData;

struct GameInterface {
	GameInterface(Float2 player, Float2 enemy){
		this->player = player;
		this->enemy = enemy;
	}

	Float2 player;
	Float2 enemy;

	bool cameraFlag = false;
	bool shutterFlag = false;

	Array<std::unique_ptr<Bullet>> bulletBuf;

	virtual ~GameInterface() = default;

	void RegisterBullet(const BulletContainer& container, BulletData data, AngelScript::asIScriptObject* behavior);
};