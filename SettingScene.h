#pragma once

#include "Scene.h"

class SettingScene : public SceneMaster::Scene {
private:
	GameData& data;
	SaveData& save;
	Texture back = Texture(GetResource(U"AS/setting.png"));

	int select = 0;

	enum Select{
		Shot,
		Rotate,
		Slow,
		Pause,
		BGM,
		Sound,
		Window,
		ShotMode,
		Mode,
		Quit
	};

public:
	SettingScene(const InitData& init) :
		IScene(init),
		data(getData()),
		save(data.save){

	}

	bool SetKey(int target) {
		auto gamepad = Gamepad(0);
		auto key = gamepad.buttons;
		int buf = -1;;
		for (auto i : step(key.size())) {
			if (key[i].down()) {
				buf = i;
				break;
			}
		}
		if (buf >= 0) {
			for (auto& i : save.gamepadNum) {
				if (buf == i) {
					std::swap(i, save.gamepadNum[target]);
					return true;
				}
			}
			save.gamepadNum[target] = buf;
			return true;
		}
		return false;
	}

	void update()override {
		switch (select) {
		default:
		case SettingScene::Shot:
			if (SetKey(0)) {
				data.ApplySetting();
				data.PlayOneShot(data.aOk);
			}
			break;
		case SettingScene::Rotate:
			if (SetKey(1)) {
				data.ApplySetting();
				data.PlayOneShot(data.aOk);
			}
			break;
		case SettingScene::Slow:
			if (SetKey(2)) {
				data.ApplySetting();
				data.PlayOneShot(data.aOk);
			}
			break;
		case SettingScene::Pause:
			if (SetKey(3)) {
				data.ApplySetting();
				data.PlayOneShot(data.aOk);
			}
			break;
		case SettingScene::BGM:
			if (data.right.down() || data.right.interval(30, 5)) {
				save.bgmVolume += 5;
				if (save.bgmVolume > 100)
					save.bgmVolume = 100;
				data.ApplySetting();
			}
			if (data.left.down() || data.left.interval(30, 5)) {
				save.bgmVolume -= 5;
				if (save.bgmVolume < 0)
					save.bgmVolume = 0;
				data.ApplySetting();
			}
			break;
		case SettingScene::Sound:
			if (data.right.down() || data.right.interval(30, 5)) {
				save.soundVolume += 5;
				if (save.soundVolume > 100)
					save.soundVolume = 100;
				data.ApplySetting();
			}
			if (data.left.down() || data.left.interval(30, 5)) {
				save.soundVolume -= 5;
				if (save.soundVolume < 0)
					save.soundVolume = 0;
				data.ApplySetting();
			}
			data.Play(data.aShutter);
			break;
		case SettingScene::Window:
			if (data.right.down()) {
				save.windowSize+= 2;
				save.windowSize %= 3;
				data.ApplySetting();
				data.PlayOneShot(data.aSelect);
			}
			if (data.left.down()) {
				save.windowSize++;
				save.windowSize %= 3;
				data.ApplySetting();
				data.PlayOneShot(data.aSelect);
			}
			break;
		case SettingScene::Mode:
			if (data.right.down() || data.left.down()) {
				save.hardMode = !save.hardMode;
				data.PlayOneShot(data.aSelect);
			}
			break;
		case SettingScene::ShotMode:
			if (data.right.down() || data.left.down()) {
				save.shotMode = !save.shotMode;
				data.PlayOneShot(data.aSelect);
			}
			break;
		case SettingScene::Quit:
			if ((data.ok | data.cancel | data.escape).down()) {
				data.isPageForward = true;
				save.Save();
				changeScene(U"Title", 1000, true);
				data.PlayOneShot(data.aCancel);
			}
			break;
		}
		if (data.up.down()) {
			select += 9;
			data.PlayOneShot(data.aSelect);
		}
		if (data.down.down()) {
			select++;
			data.PlayOneShot(data.aSelect);
		}
		select %= 10;
	}

	void draw()const override {
		Transformer2D t(Mat3x2::Scale(static_cast<double>(WindowSetting::size.y) / 960.0));

		back.draw();

		data.mk_title(U"Settings_�ݒ�").draw(Point(40, 20), Palette::Yellow);

		for (auto i : step(10)) {
			ColorF color = (i == select) ? Palette::White : Palette::Dimgray;

			String str = U"";
			String strR = U"";

			switch (i) {
			default:
			case SettingScene::Shot:
				str = U"�B�e�{�^���i����j";
				strR = U"{:0>2}"_fmt(save.gamepadNum[0] + 1);
				break;
			case SettingScene::Rotate:
				str = U"��]�{�^���i�L�����Z���j";
				strR = U"{:0>2}"_fmt(save.gamepadNum[1] + 1);
				break;
			case SettingScene::Slow:
				str = U"�ᑬ�{�^��";
				strR = U"{:0>2}"_fmt(save.gamepadNum[2] + 1);
				break;
			case SettingScene::Pause:
				str = U"�|�[�Y�{�^��";
				strR = U"{:0>2}"_fmt(save.gamepadNum[3] + 1);
				break;
			case SettingScene::BGM:
				str = U"BGM�̉���";
				strR = Format(save.bgmVolume) + U"%";
				break;
			case SettingScene::Sound:
				str = U"���ʉ��̉���";
				strR = Format(save.soundVolume) + U"%";
				break;
			case SettingScene::Window:
				str = U"�E�C���h�E�T�C�Y";
				switch (save.windowSize) {
				default:
				case 0:
					strR = U"1280x960";
					break;
				case 1:
					strR = U"960x720";
					break;
				case 2:
					strR = U"640x480";
					break;
				}
				break;
			case SettingScene::Mode:
				str = U"�Q�[�����[�h";
				if (save.hardMode)
					strR = U"�T�h���f�X";
				else
					strR = U"�G���W���C";
				break;
			case SettingScene::ShotMode:
				str = U"�ᑬ���ɎB�e�{�^���ŎB�e";
				if (save.shotMode)
					strR = U"���Ȃ�";
				else
					strR = U"����";
				break;
				break;
			case SettingScene::Quit:
				str = U"���ǂ�";
				break;
			}

			data.gothic_m(str).draw(200, 140 + i * 80, color);
			data.gothic_m(strR).draw(Arg::topRight = Point(1000, 140 + i * 80), color);
		}
	}
};


//�ݒ�ł���悤�ɂ������

/*
������ʂł���Ă�����

�𑜓x

�L�[�R���t�B�O
->�B�e�A��]�A�ᑬ�A�|�[�Y

BGM�̉��ʂ�
���ʉ��̉���

�n�[�h���[�h(���ʂ̃X�e�[�W�ł��T�h���f�X�ɂȂ�)



*/