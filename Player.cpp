#include "Player.h"
#include "Scene.h"

Player::Player(GameData& data):
	data(data){
	finderPos = GetTargetFinderPos();
}

void Player::Update(GameInterface& inter) {
	bool isShift = data.shift.pressed();
	float speed =  isShift ? shiftSpeed : normalSpeed;
	auto input = MoveInput();
	auto lastPos = pos;

	if (cameraInterval == 0) {
		if (isShift && data.ok.pressed()) {
			++chargeCount;
			if (chargeCount > 150)
				chargeCount = 150;
			cameraCharge += chargeCount / 15;
			speed = chargingSpeed;
		}
		else {
			chargeCount = 0;
			cameraCharge += 1;
		}
	}
	else {
		++cameraInterval;
		if (cameraInterval > defaultCameraInterval) {
			if (cameraCharge < recover * 10) {
				cameraCharge += 10;
			}
			else {
				cameraInterval = 0;
				recover = 0;
			}
		}
	}

	pos += speed * input;
	if (cameraCharge > cameraChargeMax)
		cameraCharge = cameraChargeMax;

	if (data.cancel.down() && !inter.cameraFlag)
		cameraAngleFlag = !cameraAngleFlag;

	pos = Clamp(pos, outerRad, data.field);

	auto buf = pos - lastPos;
	if (isShift) {
		auto v = inter.enemy - pos;
		finderCnt = 0;
		finder = AdjustRad(finder, Math::Atan2(v.y, v.x), 24_deg);
	}
	else if (buf.isZero()) {
		auto v = inter.enemy - pos;
		if (finderCnt > 0)finderCnt = 0;
		else --finderCnt;
		finder = AdjustRad(finder, Math::Atan2(v.y, v.x), Min(ToRadians(-finderCnt), 4_deg));
		if (finder == Math::Atan2(v.y, v.x))finderCnt = 0;
	}
	else {
		if (finderCnt < 0)finderCnt = 0;
		else ++finderCnt;
		finder = AdjustRad(finder, Math::Atan2(buf.y, buf.x), Min(ToRadians(finderCnt), 8_deg));
		if (finder == Math::Atan2(buf.y, buf.x))finderCnt = 0;
	}

	if (isShift && cameraCharge == cameraChargeMax) {
		finderDis += 1;
		if (finderDis > maxFinderDis)
			finderDis = maxFinderDis;
	}
	else {
		finderDis -= 2;
		if (finderDis < defaultFinderDis)
			finderDis = defaultFinderDis;
	}
	finderPos = Clamp(GetTargetFinderPos(), 30, data.field);

	if (!isShift && data.ok.down() && cameraCharge == cameraChargeMax) {
		finderCnt = 0;
		inter.cameraFlag = true;
		cameraCharge = 0;
	}
}

void Player::MoveCamera(GameInterface& inter) {
	finderDis = defaultFinderDis;
	finderPos += MoveInput() * normalSpeed;
	finderPos = Clamp(finderPos, 30, data.field);
	if (!data.ok.pressed()) {
		inter.cameraFlag= false;
		inter.shutterFlag = true;
		cameraCharge = 0;
	}
}

void Player::Draw()const {
	player.drawAt(pos);
//	Circle(finderPos, 20).draw(Palette::Red);
	//Rect(Arg::center = finderPos.asPoint(), 80, 60).rotated(finder + 90_deg).drawFrame();
	finderTex.rotated(GetCameraAngle()).drawAt(finderPos, ColorF(Palette::White).setA(0.5));
	data.gothic_s(100 * cameraCharge / cameraChargeMax).draw(finderPos + Point(12, 9), ColorF(1.0, 0.8, 0.8).setA(0.7));
}

void Player::DrawCamera()const {
	cameraTex.rotated(GetCameraAngle()).drawAt(finderPos, ColorF(Palette::White).setA(0.5));
}

Vec2 Player::MoveInput() {
	Vec2 buf = Vec2::Zero();
	if (data.right.pressed()) buf.x += 1.0;
	if (data.left.pressed()) buf.x -= 1.0;
	if (data.down.pressed()) buf.y += 1.0;
	if (data.up.pressed()) buf.y -= 1.0;
	if (buf.x != 0 && buf.y != 0) buf *= Math::Pow(2, -0.5);
	return buf;
}