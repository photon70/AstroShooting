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

	int photoNum;
	Float2 player;
	Float2 enemy;
	Array<String> debug;

	bool cameraFlag = false;
	bool shutterFlag = false;

	bool aStar1 = false;
	bool aStar2 = false;
	bool aStardust = false;

	Array<std::unique_ptr<Bullet>> bulletBuf;

	virtual ~GameInterface() = default;

	void RegisterBullet(const BulletContainer& container, BulletData data, AngelScript::asIScriptObject* behavior);
	
	void Debug(String str) {
		debug << str;
	}
};