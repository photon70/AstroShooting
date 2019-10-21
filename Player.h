#pragma once

#include "Header.h"
#include "Resource.h"

struct GameData;
struct GameInterface;

struct Player {
private:
	Float2 pos = Float2(0, 200);
	int count;

	float finder = -Math::HalfPiF;
	int finderCnt = 0;
	float finderSize = 1.0;
	const float finderSizeMin = 1.0 / 9.0;
	const int finderTimeDelay = 30;
	const int finderTime = 80;
	bool cameraAngleFlag = true;
	const float outerRad = 24;
	const float rad = 4;
	int shiftCount = 0;
	int shiftCountTime = 10;
	bool chargeFlag = false;

	bool shotMode;

	Float2 finderPos;
	int cameraChargeMax = 1000;
	int cameraCharge = cameraChargeMax * 0.6;
	int chargeCount = 0;
	bool charged = false;

	bool changed = false;

	const int defaultCameraInterval = 120;
	int recover = 0;
	int cameraInterval = 0;

	Texture player = Texture(GetResource(U"AS/player_b.png"));
	Texture finderTex = Texture(GetResource(U"AS/finder.png"));
	Texture cameraTex = Texture(GetResource(U"AS/camera.png"));
	Texture cameraHitTex = Texture(GetResource(U"AS/cameraHit.png"));

	const float defaultFinderDis = 120;
	const float maxFinderDis = 200;
	float finderDis = 60;

	GameData& data;

	const float normalSpeed = 5;
	const float shiftSpeed = 2.3;
	const float chargingSpeed = 0.5;

	Rect cameraRect = Rect(Arg::center = Point::Zero(), 360, 270);
	Rect cameraFrameRect = Rect(Arg::center = Point::Zero(), 370, 280);


	Float2 GetTargetFinderPos()const {
		return pos + Circular(finderDis, finder + 90_deg).toFloat2();
	}

	double GetCameraAngle()const {
		return finder + (cameraAngleFlag ? 90_deg : 0);
	}
public:
	Player(GameData& data, int time);

	void Update(GameInterface& inter);

	void MoveCamera(GameInterface& inter);

	void Draw()const;

	void DrawCamera()const;

	Vec2 MoveInput();

	void ResetCamera() {
		cameraCharge = 0;
		chargeCount = 0;
		SetCameraInterval();
	}

	void SetCameraInterval(float recover = 0) {
		cameraInterval = 1;
		this->recover = recover;
	}

	void ResetChargeCount() {
		chargeCount = 0;
	}

	const Float2& GetPos()const {
		return pos;
	}

	const Float2& GetFinderPos()const {
		return finderPos;
	}

	const Quad& GetCameraQuad()const {
		return cameraRect.scaled(finderSize).rotated(GetCameraAngle());
	}

	const Quad& GetCameraFrameQuad()const {
		auto r = cameraRect.scaled(finderSize);
		r.setPos(r.pos - Size(5, 5));
		r.setSize(r.size + Size(10, 10));
		return r.rotated(GetCameraAngle());
	}

	const Circle& GetCircle()const {
		return Circle(pos, rad);
	}

	bool Charging()const {
		return chargeCount > 30 && cameraCharge != cameraChargeMax;
	}

	bool Charged()const {
		return charged;
	}

	bool Changed()const {
		return changed;
	}

	bool StartCharge()const {
		return cameraInterval == defaultCameraInterval;
	}
};