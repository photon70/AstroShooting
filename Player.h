#pragma once

#include "Header.h"

struct GameData;
struct GameInterface;

struct Player {
private:
	Float2 pos = Float2(0, 200);
	float finder = -Math::HalfPiF;
	int finderCnt = 0;
	bool cameraAngleFlag = true;
	const float outerRad = 24;
	const float rad = 4;

	Float2 finderPos;
	int cameraChargeMax = 1000;
	int cameraCharge = cameraChargeMax * 0.6;
	int chargeCount = 0;

	const int defaultCameraInterval = 60;
	int recover = 0;
	int cameraInterval = 0;

	Texture player = Texture(U"AS/player_b.png");
	Texture finderTex = Texture(U"AS/finder.png");
	Texture cameraTex = Texture(U"AS/camera.png");

	const float defaultFinderDis = 120;
	const float maxFinderDis = 200;
	float finderDis = defaultFinderDis;

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
	Player(GameData& data);

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

	const Float2& GetPos()const {
		return pos;
	}

	const Float2& GetFinderPos()const {
		return finderPos;
	}

	const Quad& GetCameraQuad()const {
		return cameraRect.rotated(GetCameraAngle());
	}

	const Quad& GetCameraFrameQuad()const {
		return cameraFrameRect.rotated(GetCameraAngle());
	}

	const Circle& GetCircle()const {
		return Circle(pos, rad);
	}
};