#pragma once

#include <Siv3D.hpp>

#include "Header.h"

#include "GameInterface.h"

//弾の画像と基本性能を結びつける
struct BulletContainer {
	Texture texture;
	ColorF color = Palette::White;
	float scale = 1.0f;
	float rad = 0;
	int score = 0;
};

//弾の持つ可変データ
struct BulletData {
	Float2 pos = Float2::Zero();
	Float2 angle = Float2::Zero();
	float speed = 1.0;
	float radian = 0;
	int count = 0;
	bool autoDelete = true;
	bool deleteFlag = false;

	BulletData() = default;

	BulletData(const Float2& pPos, float pAngle, float pRad = 1.f, int pCount = 0) {
		pos = pPos;
		angle = Circular(pRad, pAngle).toFloat2();
		speed = pRad;
		radian = pAngle;
		count = -abs(pCount);
	}

	BulletData SetPos(const Float2& pPos) {
		BulletData ret(*this);
		ret.pos = pPos;
		return ret;
	}

	BulletData AddPos(const Float2& pPos) {
		BulletData ret(*this);
		ret.pos += pPos;
		return ret;
	}

	BulletData SetAngle(float pAngle, float pRad = 1.f) {
		BulletData ret(*this);
		ret.angle = Circular(pRad, pAngle).toFloat2();
		ret.speed = pRad;
		ret.radian = pAngle;
		return ret;
	}

	BulletData AddAngle(float pAngle, float pRad = 0) {
		BulletData ret(*this);
		ret.speed += pRad;
		ret.radian += pAngle;
		ret.angle = Circular(ret.speed, ret.radian).toFloat2();
		return ret;
	}

	BulletData SetCount(int pCount) {
		BulletData ret(*this);
		ret.count = -abs(pCount);
		return ret;
	}

	BulletData AutoDelete(bool flag = false) {
		BulletData ret(*this);
		ret.autoDelete = flag;
		return ret;
	}

	BulletData Set(const Float2& pPos, float pAngle, float pRad, int pCount) {
		BulletData ret(*this);
		ret.pos = pPos;
		ret.angle = Circular(pRad, pAngle).toFloat2();
		ret.speed += pRad;
		ret.radian = pAngle;
		ret.count= -abs(pCount);
		return ret;
	}
};

struct Bullet {
public:
	static ph::ScriptFunctionHandle update;
	const BulletContainer& container;
	BulletData data;
	ph::ScriptObject behavior;

	Bullet(const BulletContainer& container, BulletData& data, ph::ScriptObject behavior) :
		container(container), data(data), behavior(behavior) {

	}

	virtual ~Bullet() = default;

	void Update(ph::AdvancedScript& script, GameInterface& inter) {
		if (!behavior)data.pos += data.angle;
		else script.getObjectMethod<void(GameInterface&, BulletData&)>(update, behavior)(inter, data);
		++data.count;
	}

	void Draw()const {
		auto rad = Math::Atan2(data.angle.y, data.angle.x);
		container.texture.scaled(container.scale).rotated(rad + (data.count / 10 % 2 == 0 ? 45_deg : 0)).drawAt(data.pos, container.color);
	}

	bool IsDelete()const {
		return data.deleteFlag;
	}

	CircleF GetCircle()const {
		return CircleF(data.pos, container.rad);
	}
};