#pragma once

#include <Siv3D.hpp>

inline Rect Clamp(const Rect& r1, const Rect& r2) {
	Point buf(Clamp(r1.x, r2.x, r2.x + r2.w), Clamp(r1.y, r2.y, r2.y + r2.h));
	return Rect(buf, Point(Clamp(r1.x + r1.w, r2.x, r2.x + r2.w), Clamp(r1.y + r1.h, r2.y, r2.y + r2.h)) - buf);
}

inline double NormalizeRad(double rad) {
	auto flag = true;
	if (rad > 0) {
		while (flag) {
			flag = false;
			if (rad > Math::Pi) {
				flag = true;
				rad -= Math::TwoPi;
			}
		}
	}
	else {
		while (flag) {
			flag = false;
			if (rad < -Math::Pi) {
				flag = true;
				rad += Math::TwoPi;
			}
		}
	}
	return rad;
}

inline double AdjustRad(double sub, double obj, double speed) {
	auto d = NormalizeRad(obj - sub);
	if (Abs(d) < speed)return obj;
	if (d > 0)return sub + speed;
	else return sub - speed;
}

inline Float2 Clamp(const Float2& v, float rad, const Rect& r) {
	auto ret = v;
	ret.x = std::clamp(ret.x, (float)(r.x + rad), (float)(r.x + r.w - rad));
	ret.y = std::clamp(ret.y, (float)(r.y + rad), (float)(r.y + r.h - rad));
	return ret;
}