#pragma once

#include <Siv3D.hpp>

struct ArrowKey {
	int count = 0;

	const double deadzone = 0.5;

	enum Direc {
		Right,
		Left,
		Up,
		Down
	}direc = Right;

	ArrowKey() = default;

	ArrowKey(Direc direc) :
		direc(direc){
		count = 0;
	}

	void update(detail::Gamepad_impl& pad) {
		bool flag = false;
		switch (direc) {
		default:
		case ArrowKey::Right:
			flag = KeyRight.pressed() || (pad && pad.povRight.down()) || (pad && pad.axes[0] >= deadzone);
			break;
		case ArrowKey::Left:
			flag = KeyLeft.pressed() || (pad && pad.povLeft.down()) || (pad && pad.axes[0] <= -deadzone);
			break;
		case ArrowKey::Up:
			flag = KeyUp.pressed() || (pad && pad.povUp.down()) || (pad && pad.axes[1] <= -deadzone);
			break;
		case ArrowKey::Down:
			flag = KeyDown.pressed() || (pad && pad.povDown.down()) || (pad && pad.axes[1] >= deadzone);
			break;
		}

		if (flag) {
			++count;
		}
		else {
			count = 0;
		}
	}

	bool down()const {
		return count == 1;
	}

	bool pressed()const {
		return count > 0;
	}

	bool interval(int wait, int time) {
		return count >= wait && (count - wait) % time == 0;
	}
};