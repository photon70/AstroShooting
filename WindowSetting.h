#pragma once

#include <Siv3D.hpp>

struct WindowSetting {
	static Size size;

	static const Size& GetSize() {
		return size;
	}
	static Rect GetRect() {
		return Rect(Point::Zero(), size);
	}
};