#include "Player.h"
#include "Scene.h"

Player::Player(GameData& data, int time):
	data(data){
	finderPos = GetTargetFinderPos();
	shotMode = data.save.shotMode;
	chargeCount = -time;
}

void Player::Update(GameInterface& inter) {
	charged = false;
	changed = false;
	if (data.shift.pressed() || chargeFlag) {
		if (shiftCount < 0)
			shiftCount = 0;
		++shiftCount;
	}
	else {
		if (shiftCount > 0)
			shiftCount = -shiftCountTime;
		++shiftCount;
		if (shiftCount > 0)
			shiftCount = 0;
	}
	float speed = shiftCount > 0 ? shiftSpeed : normalSpeed;
	auto input = MoveInput();
	auto lastPos = pos;

	if (cameraInterval == 0) {
		if (chargeFlag) {
			if (cameraCharge < cameraChargeMax) {
				++chargeCount;
				if (chargeCount > 150)
					chargeCount = 150;
				if(chargeCount > 0)
					cameraCharge += chargeCount / 15;
				if (cameraCharge >= cameraChargeMax)
					charged = true;
			}
		}
		else {
			chargeCount++;
			if (cameraCharge < cameraChargeMax) {
				if (chargeCount > 0)
					cameraCharge += chargeCount / 15;
				if(chargeCount > 30)
					chargeCount = 30;
				if (cameraCharge >= cameraChargeMax)
					charged = true;
			}
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
	if (shotMode) {
		chargeFlag = shiftCount > 0 && data.ok.pressed();
	}
	else {
		if (cameraCharge < cameraChargeMax && data.ok.pressed()) {
			chargeFlag = true;
		}
		if (!data.ok.pressed()) {
			chargeFlag = false;
		}
	}

	if (chargeFlag)
		speed = chargingSpeed;

	pos += speed * input;
	if (input != Float2::Zero()) {
		if (speed == normalSpeed)
			count += 4;
		if (speed == shiftSpeed)
			count += 2;
		if (speed == chargingSpeed)
			count += 1;
	}
	if (cameraCharge > cameraChargeMax) {
		cameraCharge = cameraChargeMax;
	}

	if (data.cancel.down() && !inter.cameraFlag) {
		cameraAngleFlag = !cameraAngleFlag;
		changed = true;
	}

	pos = Clamp(pos, outerRad, data.field);

	auto buf = pos - lastPos;
	if (shiftCount > 0) {
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

	if (cameraCharge == cameraChargeMax) {
		if (shiftCount > 0 && finderDis >= defaultFinderDis) {
			finderDis += 1;
			if (finderDis > maxFinderDis)
				finderDis = maxFinderDis;
		}
		else {
			if (finderDis > defaultFinderDis) {
				finderDis -= 2;
				if (finderDis < defaultFinderDis)
					finderDis = defaultFinderDis;
			}
			if (finderDis < defaultFinderDis) {
				finderDis += 4;
				if (finderDis > defaultFinderDis)
					finderDis = defaultFinderDis;
			}
		}
	}
	else {
		finderDis = 60;
	}
	auto dis = Float2(inter.enemy - pos).length();
	if (finderDis > dis)
		finderDis = dis;
	finderPos = Clamp(GetTargetFinderPos(), 30, data.field);

	if ((!shotMode || shiftCount <= 0) && data.ok.down() && cameraCharge == cameraChargeMax) {
		finderCnt = 0;
		inter.cameraFlag = true;
		chargeCount = 0;
		cameraCharge = 0;
		finderSize = 1.0;
	}
}

void Player::MoveCamera(GameInterface& inter) {
	charged = false;
	changed = false;
	finderDis = defaultFinderDis;
	finderPos += MoveInput() * normalSpeed;
	++chargeCount;
	finderPos = Clamp(finderPos, 30, data.field);
	if(chargeCount > finderTimeDelay)
		finderSize -= (1.0 - finderSizeMin) / (finderTime - finderTimeDelay);
	if (finderSize < finderSizeMin) {
		finderSize = finderSizeMin;
		inter.cameraFlag = false;
		ResetCamera();
	}
	if (!data.ok.pressed()) {
		inter.cameraFlag= false;
		inter.shutterFlag = true;
		chargeCount = 0;
		cameraCharge = 0;
	}
}

void Player::Draw()const {
	player((count / 30 % 4) * 48, 0, Size(48, 64)).drawAt(pos);
	if (shiftCount > -shiftCountTime)
		cameraHitTex.drawAt(pos, ColorF(Palette::White).setA((float)Abs(shiftCount) / (float)shiftCountTime));
//	Circle(finderPos, 20).draw(Palette::Red);
	//Rect(Arg::center = finderPos.asPoint(), 80, 60).rotated(finder + 90_deg).drawFrame();
	ColorF color = Palette::White;
	ColorF color2(1.0, 0.8, 0.8);
	if (cameraCharge < cameraChargeMax) {
		color.setA(0.5);
		color2.setA(0.7);
	}
	finderTex.rotated(GetCameraAngle()).drawAt(finderPos, color);
	data.gothic_s(100 * cameraCharge / cameraChargeMax).draw(finderPos + Point(12, 9), color2);
}

void Player::DrawCamera()const {
	cameraTex.scaled(finderSize).rotated(GetCameraAngle()).drawAt(finderPos, ColorF(Palette::White).setA(0.5));
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