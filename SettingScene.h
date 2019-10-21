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

		data.mk_title(U"Settings_設定").draw(Point(40, 20), Palette::Yellow);

		for (auto i : step(10)) {
			ColorF color = (i == select) ? Palette::White : Palette::Dimgray;

			String str = U"";
			String strR = U"";

			switch (i) {
			default:
			case SettingScene::Shot:
				str = U"撮影ボタン（決定）";
				strR = U"{:0>2}"_fmt(save.gamepadNum[0] + 1);
				break;
			case SettingScene::Rotate:
				str = U"回転ボタン（キャンセル）";
				strR = U"{:0>2}"_fmt(save.gamepadNum[1] + 1);
				break;
			case SettingScene::Slow:
				str = U"低速ボタン";
				strR = U"{:0>2}"_fmt(save.gamepadNum[2] + 1);
				break;
			case SettingScene::Pause:
				str = U"ポーズボタン";
				strR = U"{:0>2}"_fmt(save.gamepadNum[3] + 1);
				break;
			case SettingScene::BGM:
				str = U"BGMの音量";
				strR = Format(save.bgmVolume) + U"%";
				break;
			case SettingScene::Sound:
				str = U"効果音の音量";
				strR = Format(save.soundVolume) + U"%";
				break;
			case SettingScene::Window:
				str = U"ウインドウサイズ";
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
				str = U"ゲームモード";
				if (save.hardMode)
					strR = U"サドンデス";
				else
					strR = U"エンジョイ";
				break;
			case SettingScene::ShotMode:
				str = U"低速中に撮影ボタンで撮影";
				if (save.shotMode)
					strR = U"しない";
				else
					strR = U"する";
				break;
				break;
			case SettingScene::Quit:
				str = U"もどる";
				break;
			}

			data.gothic_m(str).draw(200, 140 + i * 80, color);
			data.gothic_m(strR).draw(Arg::topRight = Point(1000, 140 + i * 80), color);
		}
	}
};


//設定できるようにするもの

/*
同じ画面でやってもいい

解像度

キーコンフィグ
->撮影、回転、低速、ポーズ

BGMの音量と
効果音の音量

ハードモード(普通のステージでもサドンデスになる)



*/