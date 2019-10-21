#pragma once

#include "Scene.h"
#include <Windows.h>
#include <shellapi.h>

struct PhotoUnit {
	Image photo;
	Texture tex;
	int score = 0;

	float enemyCenterBonus = 1.0;
	float medalBonus = 1.0;

	const int maxRecoverScore = 50000;
	const int maxRecover = 60;

	PhotoUnit(Image photo) :photo(photo) {}

	int GetRecoverScore() {
		return maxRecover * std::min(score, maxRecoverScore) / maxRecoverScore;
	}

	int GetScore()const {
		return score * enemyCenterBonus * medalBonus;
	}
};

struct Flash {
	const float hitMax = 0.1;
	const float cameraHitMax = 0.2;
	const float flashMax = 1;
	const float rise = hitMax / 50.f;
	const float fall = 0.02;
	float least = 0;
	float max = cameraHitMax;
	float now = 0;

	void Update() {
		if (now < least) {
			now += rise;
			if (now > least)
				now = least;
		}
		if (now > least) {
			now -= fall;
			if (now < least)
				now = least;
		}
	}

	void Hit() {
		if (least < hitMax) {
			least = hitMax;
			now = least;
		}
		if (least < max && now <= least) {
			least += rise;
			if (least > max)
				least = max;
		}
		now += 0.02;
	}

	void HitCamera() {
		max = cameraHitMax;
		if (least < cameraHitMax)
			least = cameraHitMax;
		now = flashMax;
	}

	ColorF GetNowColor()const {
		return ColorF(now, now, now);
	}

	float GetNow()const {
		return now;
	}
};

class GameScene : public SceneMaster::Scene {
private:
	GameData& data;
	Texture back;
	Texture frame;
	Texture tape = Texture(GetResource(U"AS/tape.png"));

	bool cameraFlag = false;
	bool shutterFlag = false;
	bool stardustFlag = false;

	int count = 0;
	const int cutinTime = 120;

	int enCount = 0;

	Player player;
	Array<std::unique_ptr<Bullet>> bullet;
	std::unique_ptr<Enemy> enemy;
	Flash flash;

	Array<PhotoUnit> photo;
	int stageNum;
	double lastTime;
	int targetNum;
	bool suddenDeath;
	bool tutorialFlag;
	int hiScore;
	int score = 0;
	String cameraman;
	int savePhoto = -1;
	bool updatePhotoFlag = false;

	int stateCnt = 0;
	const int transCnt = 20;
	int cursor = 0;
	enum State {
		Normal,
		Pause,
		Clear,
		Over,
		Printer,
	}state = Normal;

public:
	GameScene(const InitData& init) :
		IScene(init),
		data(getData()),
		back(GetResource(U"AS/back.png")),
		frame(GetResource(U"AS/frame.png")),
		player(data, cutinTime) {
		stageNum = data.GetStageNum();
		if (stageNum >= 0)
			enemy = std::make_unique<Enemy>(data.enemy[stageNum], data.script.getObjectFactory(data.enemy[stageNum].behaviorName)());
		const auto& e = data.enemy[stageNum];
		lastTime = e.timeLimit;
		targetNum = e.targetNumber;
		suddenDeath = e.stageType == Enemy::SuddenDeath || data.save.hardMode;
		tutorialFlag = e.stageType == Enemy::Tutorial;
		hiScore = data.save[stageNum].hiScore;
		bullet.reserve(2000);

		data.aTitle.stop();
		data.PlayBGM(data.aBGM[stageNum / 6]);
	}

	~GameScene() {
		auto& s = data.save[stageNum];
		s.total += photo.size();
		data.save.Save();
		data.aBGM[stageNum / 6].stop();
		data.aStardust.stop();
	}

	void ChangeState(State s) {
		state = s;
		stateCnt = transCnt;
		cursor = 0;
		if (s == Normal) {
			if (stardustFlag)
				data.Play(data.aStardust);
		}
		else {
			if (stardustFlag)
				data.aStardust.pause();
			if (s == Clear && updatePhotoFlag) {
				cursor = 3;
			}
		}
	}

	double GetProgress()const {
		return Math::Abs(static_cast<double>(stateCnt) / static_cast<double>(transCnt));
	}

	void CreatePhoto() {
		Image image(480, 640);
		for (auto i : step(photo[savePhoto].photo.size())) {
			image[i] = photo[savePhoto].photo[i];
		}
		data.mk_menu_l(data.enemy[stageNum].name).overwrite(image, Point(240 - data.mk_menu_l(data.enemy[stageNum].name).region().w / 2, 480), Palette::Black);
		data.mk_menu_s(U"撮影者").overwrite(image, 80, 552, Palette::Black);
		data.mk_menu_s(U"得点").overwrite(image, 80, 598, Palette::Black);
		data.mk_menu_s(cameraman).overwrite(image, Point(400 - data.mk_menu_s(cameraman).region().w, 552), Palette::Black);
		data.mk_menu_s(score).overwrite(image, Point(400 - data.mk_menu_s(score).region().w, 598), Palette::Black);
		image.savePNG(U"AS/print.png");
	}

	void update()override {
		if (data.updateFlag) {
			bullet.clear();
		}
		if (data.updateFlag) changeScene(U"Game");
		switch (state)
		{
		default:
		case GameScene::Normal:
			updateNormal();
			++count;
			lastTime -= System::DeltaTime();
			if (count == 10) {
				data.PlayOneShot(data.aStar1);
			}
			for (auto i : step(targetNum)) {
				if (count == 20 + i * 10) {
					data.PlayOneShot(data.aStar2);
				}
			}
			if (lastTime < 0) {
				lastTime = 0;
				ChangeState(Over);
				count = 0;
				data.PlayOneShot(data.aOver);
			}
			if (state == Normal && !cameraFlag && !shutterFlag && data.escape.down()) {
				ChangeState(Pause);
				data.PlayOneShot(data.aOk);
			}
			break;
		case GameScene::Pause:
			if (stateCnt > 0)--stateCnt;
			if (stateCnt < 0)--stateCnt;
			if (stateCnt == -transCnt)
				ChangeState(Normal);
			if (stateCnt == 0) {
				if (data.up.down()) {
					cursor += 3;
					data.PlayOneShot(data.aSelect);
				}
				if (data.down.down()) {
					cursor++;
					data.PlayOneShot(data.aSelect);
				}
				cursor %= 4;
				if (data.escape.down()) {
					stateCnt = -1;
					data.PlayOneShot(data.aCancel);
				}
				if (data.ok.down()) {
					switch (cursor)
					{
					default:
					case 0:
						data.PlayOneShot(data.aOk);
						stateCnt = -1;
						break;
					case 1:
						data.isPageForward = false;
						data.PlayOneShot(data.aOk);
						changeScene(U"Menu");
						break;
					case 2:
						data.PlayOneShot(data.aOk);
						Manual();
						break;
					case 3:
						data.PlayOneShot(data.aOk);
						changeScene(U"Game");
						break;
					}
				}
			}
			break;
		case GameScene::Clear:
			if (stateCnt > 0)--stateCnt;
			if (stateCnt < 0)--stateCnt;
			++count;
			if (stateCnt == 0 && count > 60) {
				if (updatePhotoFlag) {
					if (data.up.down()) {
						cursor += 3;
						data.PlayOneShot(data.aSelect);
					}
					if (data.down.down()) {
						cursor++;
						data.PlayOneShot(data.aSelect);
					}
					cursor %= 4;
				}
				else {
					if (data.up.down()) {
						cursor += 2;
						data.PlayOneShot(data.aSelect);
					}
					if (data.down.down()) {
						cursor++;
						data.PlayOneShot(data.aSelect);
					}
					cursor %= 3;
				}

				if (data.left.down()) {
					savePhoto += photo.size() - 1;
					data.PlayOneShot(data.aSelect);
				}
				if (data.right.down()) {
					savePhoto++;
					data.PlayOneShot(data.aSelect);
				}
				savePhoto %= photo.size();
				if (data.ok.down()) {
					switch (cursor)
					{
					default:
					case 0:
						data.PlayOneShot(data.aOk);
						data.isPageForward = false;
						changeScene(U"Menu");
						break;
					case 1:
						if (data.StageAvailable(data.GetStageNum())) {
							data.PlayOneShot(data.aOk);
							data.GoNextStage();
							changeScene(U"Game");
						}
						else {
							data.PlayOneShot(data.aCancel);
						}
						break;
					case 2:
						data.PlayOneShot(data.aOk);
						changeScene(U"Game");
						break;
					case 3:
						data.PlayOneShot(data.aOk);
						if (WritePlayerName()) {
							SavePhoto(savePhoto, cameraman);
							data.save.Save();
							cursor = 0;
						}
						break;
					}
				}
			}
			break;
		case GameScene::Over:
			if (stateCnt > 0)--stateCnt;
			if (stateCnt < 0)--stateCnt;
			++count;
			if (stateCnt == 0) {
				if (data.up.down()) {
					cursor += 2;
					data.PlayOneShot(data.aSelect);
				}
				if (data.down.down()) {
					cursor++;
					data.PlayOneShot(data.aSelect);
				}
				cursor %= 3;
				if (data.ok.down()) {
					switch (cursor)
					{
					default:
					case 0:
						data.PlayOneShot(data.aOk);
						changeScene(U"Game");
						break;
					case 1:
						data.isPageForward = false;
						data.PlayOneShot(data.aOk);
						changeScene(U"Menu");
						break;
					case 2:
						data.PlayOneShot(data.aOk);
						Manual();
						break;
					}
				}
			}
			break;
		case GameScene::Printer:
			if (stateCnt > 0)--stateCnt;
			if (stateCnt < 0)--stateCnt;
			++count;
			if (stateCnt == -transCnt)
				ChangeState(Clear);
			if (stateCnt == 0 && count > 60) {
				if (data.up.down() || data.down.down()) {
					cursor++;
					data.PlayOneShot(data.aSelect);
				}
				cursor %= 2;
				if (data.left.down()) {
					savePhoto += photo.size() - 1;
					data.PlayOneShot(data.aSelect);
				}
				if (data.right.down()) {
					savePhoto++;
					data.PlayOneShot(data.aSelect);
				}
				savePhoto %= photo.size();
				if (data.ok.down()) {
					switch (cursor)
					{
					default:
					case 0:
						data.PlayOneShot(data.aOk);
						if (WritePlayerName()) {
							//印刷処理
							CreatePhoto();
							ShellExecute(reinterpret_cast<HWND>(Windows::Window::GetHWND()), L"open", L"PrintApp.exe", nullptr, nullptr, SW_SHOW);
							stateCnt = -1;
						}
						break;
					case 1:
						stateCnt = -1;
						data.PlayOneShot(data.aOk);
						break;
					}
				}
			}
			break;
		}
	}

	void updateNormal() {
		//撮影処理＆クリア判定
		if (shutterFlag) {
			data.Play(data.aShutter);
			photo << PhotoUnit(ScreenCapture::GetFrame().clipped(Rect(Arg::center = WindowSetting::GetRect().center().asPoint(), 480, 480)));
			auto& p = photo.back();
			auto q = player.GetCameraQuad();
			q.moveBy(Point(240, 240));
			auto qf = player.GetCameraFrameQuad();
			qf.moveBy(Point(240, 240));
			for (auto s : step(p.photo.size())) {
				if (!q.intersects(s)) {
					p.photo[s].setA(0);
					if (qf.intersects(s))
						p.photo[s] = Palette::White;
				}
			}
			p.tex = Texture(p.photo);
			//		p.savePNG(U"pic.png");
			q = player.GetCameraQuad();
			q.moveBy(player.GetFinderPos());
			//スコア計算
			for (auto& b : bullet) {
				if (b && q.intersects(b->data.pos)) {
					p.score += b->container->score;
					b->data.deleteFlag = true;
				}
			}
			if (q.intersects(enemy->data.pos)) {
				p.score += 1000;

				player.SetCameraInterval(p.GetRecoverScore() / 2);
				p.enemyCenterBonus = Max(1.0, 1.5 - (enemy->data.pos - player.GetFinderPos()).length() / 300);
				p.medalBonus = 2.0;
				if (flash.now >= flash.hitMax)
					p.medalBonus = 1.5 - 0.5 * (flash.now - flash.hitMax) / (flash.cameraHitMax - flash.hitMax);

				score += p.GetScore();
				if (savePhoto == -1 || photo[savePhoto].GetScore() <= photo.back().GetScore()) {
					savePhoto = photo.size() - 1;
				}
				if (hiScore < score) {
					hiScore = score;
					updatePhotoFlag = true;
				}

				if (photo.size() == targetNum && state == Normal) {//クリア判定
					if (data.printerFlag) {
						ChangeState(Printer);
					}
					else {
						ChangeState(Clear);
					}
					count = 0;
					auto& s = data.save[stageNum];
					s.hiScore = hiScore;
					if (s.score == 0)
						SavePhoto(savePhoto);
					data.Play(data.aClear);
					data.save.Save();
				}

			}
			else {
				//TODO
				player.SetCameraInterval(p.GetRecoverScore());
				photo.pop_back();
			}

			shutterFlag = false;
		}

		//ここから通常処理
		GameInterface inter(player.GetPos(), enemy->data.pos);
		inter.photoNum = photo.size();
		inter.cameraFlag = cameraFlag;
		inter.aStardust = data.aStardust.isPlaying();

		//	if (count >= cutinTime) {
		if (!cameraFlag) {
			data.aMove.stop();
			player.Update(inter);
		}
		else {
			player.MoveCamera(inter);
			data.aMove.setSpeed(0.3);
			data.Play(data.aMove);
		}
		//	}
		if (player.Charging()) {
			data.aCharge.setSpeed(0.2);
			data.Play(data.aCharge);
		}
		else {
			data.aCharge.stop();
		}
		if (player.Charged()) {
			data.aCharged.setSpeed(0.8);
			data.Play(data.aCharged);
		}
		if (player.Changed())
			data.PlayOneShot(data.aChange);
		if (count >= cutinTime && (!cameraFlag || count % 4 == 0)) {
			flash.Update();
			enemy->Update(data.script, inter);
			for (auto& b : bullet) {
				if (b) {
					if (b->data.count > 0) {
						b->Update(data.script, inter);
					}
					else {
						++b->data.count;
					}
					if (b->data.autoDelete && !data.field.intersects(b->GetCircle())) {
						b->data.deleteFlag = true;
					}
					else {
						if (b->GetCircle().intersects(player.GetCircle())) {
							if (inter.cameraFlag) {
								flash.HitCamera();
								inter.cameraFlag = false;
								inter.shutterFlag = false;
								player.ResetCamera();
								data.Play(data.aFlash);
							}
							else {
								flash.Hit();
								player.ResetChargeCount();
								data.PlayOneShot(data.aHit);
							}
							if (suddenDeath) {
								ChangeState(Over);
								count = 0;
								data.PlayOneShot(data.aOver);
							}
						}

					}
				}
			}
		}

		/*if (data.escape.down()) {
			data.isPageForward = false;
			changeScene(U"Menu");
		}*/

		data.script.getFunction<void(GameInterface&)>(U"Loop")(inter);

		ClearPrint();
		Print << bullet.size();
		RegisterUnit/*<Bullet>*/(bullet, inter.bulletBuf);

		for (auto& d : inter.debug) {
			Print << d;
		}

		cameraFlag = inter.cameraFlag;
		shutterFlag = inter.shutterFlag;
		stardustFlag = inter.aStardust;
		if (inter.aStar1)
			data.PlayOneShot(data.aStar1);
		if (inter.aStar2)
			data.PlayOneShot(data.aStar2);
		if (inter.aStardust)
			data.Play(data.aStardust);
		else
			data.aStardust.stop();

		if (shutterFlag) {
			ScreenCapture::RequestCurrentFrame();
		}
		if (tutorialFlag)
			Tutorial();
	}

	void draw()const override {
		if (!shutterFlag) {
			Transformer2D t(Mat3x2::Translate(WindowSetting::GetRect().center()).scaled(static_cast<double>(WindowSetting::size.y) / 960.0, WindowSetting::GetRect().center()));

			back.drawAt(Float2::Zero());
			//	data.field.draw(flash.GetNowColor());
			{
				//	RenderStateBlock2D blend(BlendState::Additive);
				enemy->Draw(data.script);
			}
			player.Draw();
			{
				//	RenderStateBlock2D blend(BlendState::Additive);
				for (auto& b : bullet) {
					if (b && b->data.count > 0)
						b->Draw(data.script);
				}
			}

			if (count < cutinTime && (state == Normal || state == Pause)) {
				auto e = enemy->data.pos;
				auto d = data.enemy[stageNum];
				ColorF color(Palette::White);
				ColorF color2(0.8, 0.8, 0.8);
				color.setA(1 * Easing::Linear(Min(1.f, (float)count / 10)));
				color2.setA(0.6 * Easing::Linear(Min(1.f, (float)count / 10)));
				if (count >= cutinTime - 10) {
					color.setA(1 * Easing::Linear(Min(1.f, (float)(cutinTime - count) / 10)));
					color2.setA(0.6 * Easing::Linear(Min(1.f, (float)(cutinTime - count) / 10)));
				}
				int rad = 1000 - 860 * Easing::Linear(Min(1.f, (float)count / 10));
				Line(e.movedBy(-rad, 0), e.movedBy(rad, 0)).draw(4, color2);
				Line(e.movedBy(0, -rad), e.movedBy(0, rad)).draw(4, color2);
				//	Circle(e, rad).drawFrame(0, 1000, color);
				Circle(e, rad).drawFrame(0, 4, color2);
				String attach;
				switch (d.stageType) {
				default:
				case Enemy::Normal:
					break;
				case Enemy::SuddenDeath:
					attach = U"一度も弾に当たらずに\n";
					break;
				case Enemy::Tutorial:
					attach = U"練習のために\n";
					break;
				}
				data.mk_menu_l(U"{}{}を\n{}枚撮ろう!!!"_fmt(attach, d.nickname, d.targetNumber)).draw(Arg::topCenter = Vec2(0, 0), color);
			}
			/////////////////////ここまでフレーム内
			data.field.draw(ColorF(Palette::White).setA(flash.GetNow()));

			if (cameraFlag)
				player.DrawCamera();

			data.gothic_l(U"{:.1f}"_fmt(lastTime)).drawAt(Vec2(0, -415).movedBy(1.5, 2), Palette::Dimgray);
			data.gothic_l(U"{:.1f}"_fmt(lastTime)).drawAt(Vec2(0, -415));

			if (state != State::Normal) {
				data.field.draw(ColorF(Palette::Black).setA((1 - GetProgress()) * 0.5));

				String title;
				String choice[4];
				switch (state) {
				case GameScene::Pause:
					title = U"Pause";
					choice[0] = U"つづける";
					choice[1] = U"タイトルにもどる";
					choice[2] = U"あそびかた";
					choice[3] = U"さいしょからはじめる";
					break;
				case GameScene::Clear:
					title = U"Mission Complete";
					choice[0] = U"タイトルにもどる";
					choice[1] = U"つぎのステージへすすむ";
					choice[2] = U"さいしょからはじめる";
					choice[3] = U"サインしてほぞん";
					break;
				case GameScene::Over:
					title = U"Mission Failed";
					choice[0] = U"さいちょうせん";
					choice[1] = U"タイトルにもどる";
					choice[2] = U"あそびかた";
					choice[3] = U"";
					break;
				case GameScene::Printer:
					title = U"Mission Complete";
					choice[0] = U"プリントする";
					choice[1] = U"プリントしない";
					choice[2] = U"";
					choice[3] = U"";
					break;
				}
				data.mk_pause_title(title).draw(Vec2(-300 + GetProgress() * 100, 120), ColorF(Palette::White).setA(1 - GetProgress()));
				for (auto i : step(4)) {
					ColorF c = i == cursor ? Palette::White : Palette::Gray;
					if (state == Clear && i == 3 && !updatePhotoFlag)
						c *= 0.5;
					c.setA(1 - GetProgress());
					data.gothic_pause(choice[i]).draw(Vec2(-260 + GetProgress() * 100, 200 + 50 * i), c);
				}
			}
			if (state == Clear || state == Printer) {
				ColorF c = Palette::White;
				c.setA(1 - GetProgress());
				photo[savePhoto].tex.drawAt(0, -120, c);
			}
			data.gothic_s(U"HiScore").draw(Vec2(-350, -450).movedBy(1.7, 1.2), Palette::Dimgray);
			data.gothic_s(U"Score").draw(Arg::topRight = Vec2(350, -450).movedBy(1.5, 1), Palette::Dimgray);
			data.gothic_pause(hiScore).draw(Vec2(-350, -430).movedBy(1.7, 1.2), Palette::Dimgray);
			data.gothic_pause(score).draw(Arg::topRight = Vec2(350, -430).movedBy(1.5, 1), Palette::Dimgray);
			data.gothic_s(U"HiScore").draw(Vec2(-350, -450));
			data.gothic_s(U"Score").draw(Arg::topRight = Vec2(350, -450));
			data.gothic_pause(hiScore).draw(Vec2(-350, -430));
			data.gothic_pause(score).draw(Arg::topRight = Vec2(350, -430));

			frame.drawAt(Float2::Zero());

			for (auto i : step(targetNum)) {
				auto v = Point(i < 5 ? -500 : 500, 160 * (i % 5) - 320);
				if (i >= photo.size() && (state != Normal && state != Pause || count > 20 + 10 * i))
				tape.drawAt(v);
			}

			for (auto i : step(targetNum)) {
				auto v = Point(i < 5 ? -500 : 500, 160 * (i % 5) - 320);
				if (i < photo.size()) {
					double scale = 0.5;
					photo[i].tex.scaled(scale).drawAt(v);
					ColorF c = i == savePhoto ? Palette::Pink : Palette::Lightgreen;
					data.gothic_s(photo[i].GetScore()).draw(Arg::bottomRight = v + Point(100, 80).movedBy(1.7, 1.2), Palette::Black);
					data.gothic_s(photo[i].GetScore()).draw(Arg::bottomRight = v + Point(100, 80), c);
				}
			}
			if (state == Clear || state == Printer) {
				auto v = Point(savePhoto < 5 ? -500 : 500, 160 * (savePhoto % 5) - 320);
				double scale = 0.7;
				photo[savePhoto].tex.scaled(scale).drawAt(v, ColorF(1.0, 1.0, pow(sin(ToRadians(count) * 5), 2)));
				ColorF c = Palette::Pink;
				data.gothic_s(photo[savePhoto].GetScore()).draw(Arg::bottomRight = v + Point(100, 80).movedBy(1.7, 1.2), Palette::Black);
				data.gothic_s(photo[savePhoto].GetScore()).draw(Arg::bottomRight = v + Point(100, 80), c);

				data.gothic_s(U"base point {}"_fmt(photo[savePhoto].score)).draw(Point(-350, -360).movedBy(1.7, 1.2), Palette::Dimgray);
				data.gothic_s(U"base point {}"_fmt(photo[savePhoto].score)).draw(-350, -360);
				data.gothic_s(U"center bonus x{:.2f}"_fmt(photo[savePhoto].enemyCenterBonus)).draw(Point(-350, -330).movedBy(1.7, 1.2), Palette::Dimgray);
				data.gothic_s(U"center bonus x{:.2f}"_fmt(photo[savePhoto].enemyCenterBonus)).draw(-350, -330);
				data.gothic_s(U"dark bonus x{:.2f}"_fmt(photo[savePhoto].medalBonus)).draw(Point(-350, -300).movedBy(1.7, 1.2), Palette::Dimgray);
				data.gothic_s(U"dark bonus x{:.2f}"_fmt(photo[savePhoto].medalBonus)).draw(-350, -300);
				data.gothic_s(U"total score").draw(Point(-350, -270).movedBy(1.7, 1.2), Palette::Dimgray);
				data.gothic_s(U"total score").draw(-350, -270, Palette::Pink);
				data.gothic_s(U" {}"_fmt(photo[savePhoto].GetScore())).draw(Point(-350, -240).movedBy(1.7, 1.2), Palette::Dimgray);
				data.gothic_s(U" {}"_fmt(photo[savePhoto].GetScore())).draw(-350, -240, Palette::Pink);
				if (cameraman) {
					data.gothic_s(U"cameraman").draw(Point(-350, -180).movedBy(1.7, 1.2), Palette::Dimgray);
					data.gothic_s(U"cameraman").draw(Point(-350, -180), Palette::White);
					data.gothic_s(U" {}"_fmt(cameraman)).draw(Point(-350, -150).movedBy(1.7, 1.2), Palette::Dimgray);
					data.gothic_s(U" {}"_fmt(cameraman)).draw(Point(-350, -150), Palette::Lightgreen);
				}
			}
		}
		else {
			WindowSetting::GetRect().draw(flash.GetNowColor());
			Transformer2D t(Mat3x2::Translate(WindowSetting::GetRect().center() - player.GetFinderPos()));

			back.drawAt(Float2::Zero());
			//data.field.draw(flash.GetNowColor());
			{
				//	RenderStateBlock2D blend(BlendState::Additive);

				enemy->Draw(data.script);

				for (auto& b : bullet) {
					if (b && b->data.count > 0)
						b->Draw(data.script);
				}
			}
			data.field.draw(ColorF(Palette::White).setA(flash.GetNow()));
		}
	}

	//	template<class T>
	void RegisterUnit(Array<std::unique_ptr<Bullet>>& to, Array<std::unique_ptr<Bullet>>& from) {
		////	bool flag = false;
		////	for (auto it = to.rbegin(); it != to.rend();) {
		////		if (it->get()->IsDelete()) {
		////			while(!flag && to.front()->IsDelete()) {
		////				flag = to.front() == *it;
		////				to.pop_front();
		////			}
		////			if (flag)break;
		////			it->swap(to.front());
		////			to.pop_front();
		////		}
		////		else {
		////			++it;
		////		}
		////	}
		//////	to.reserve(to.size() + from.size());
		////	to.insert(to.end(),
		////		std::make_move_iterator(from.begin()),
		////		std::make_move_iterator(from.end()));
		enCount += from.size();

		auto fromIt = from.begin();
		auto fromItEnd = from.end();
		for (auto& toIt : to) {
			if (!toIt || toIt->IsDelete()) {
				if (toIt && toIt->IsDelete()) {
					enCount--;
				}
				if (fromIt == fromItEnd) {
					toIt.reset(nullptr);
				}
				else {
					toIt.swap(*fromIt);
					++fromIt;
				}
			}
		}
		for (; fromIt != fromItEnd; ++fromIt) {
			to.emplace_back(fromIt->release());
		}
		Print << enCount;

		//Arrayで管理。先頭から消去するかを判断していって、登録するものがあるならswapないなら中身を消す。
	}

	void SavePhoto(int num, String name = U"") {
		auto& s = data.save[stageNum];
		auto& p = photo[num];
		s.score = p.GetScore();
		s.cameraman = name;
		p.photo.savePNG(U"AS/stage{}.png"_fmt(stageNum + 1));
	}

	void Tutorial() {
		int type = 0;
		if (count == cutinTime) {
			type = 0;
		}
		else if (photo.size() == 0 && player.Charged()) {
			type = 1;
		}
		else if (photo.size() <= 1 && player.StartCharge()) {
				type = 2;
		}
		else if (photo.size() == 1 && player.Charged()) {
			type = 3;
		}
		else if (photo.size() == 2 && player.Charged()) {
			type = 4;
		}
		else if (photo.size() == 3 && player.Charged()) {
			type = 5;
		}
		else {
			return;
		}

		data.PlayOneShot(data.aOk);
		draw();
		int count = 0;

		int transTime = 30;

		while (System::Update()) {
			data.Update();
			++count;

			if (count > transTime && data.ok.down()) {
				data.PlayOneShot(data.aCancel);
				return;
			}

			draw();

			String str;
			switch (type) {
			default:
			case 0:
				str = U"スティック(方向キー）で移動\n\n低速ボタン(Shiftキー)を押していると\n移動が遅くなるよ";
				break;
			case 1:
				str = U"撮影ボタン(Zキー)を押すと写真が撮れる。\n天体を写そう。";
				break;
			case 2:
				str = U"ファインダーに書いてある\n数字が100の時にしか写真は撮れない\n\n写真が撮れないときに撮影ボタン\nを押すと数字が速く増える";
				break;
			case 3:
				str = U"光弾を写真に入れると、消すことができるぞ！\n\n天体と光弾を一緒に写そう。";
				break;
			case 4:
				str = U"撮影ボタン（Zキー）を押している間に\nスティック（方向キー）を入力すると\n撮る場所を動かせるぞ！\n\n天体の方にずらして撮ってみよう。";
				break;
			case 5:
				str = U"回転ボタン（Xキー）を押すたびに\nカメラの向きが変わるぞ!\n\n基本はタテの方がやりやすい。";
				break;
			}

			Transformer2D t(Mat3x2::Translate(WindowSetting::GetRect().center()).scaled(static_cast<double>(WindowSetting::size.y) / 960.0, WindowSetting::GetRect().center()));
			data.gothic_manual(str).regionAt().stretched(20, 20).rounded(20).draw(ColorF(Palette::White).setA(0.8));
			data.gothic_manual(str).drawAt(Point::Zero(), Palette::Black);
		}
	}

	void Manual() {
		draw();
		int count = 1;
		bool changeTypeFlag = true;

		int transTime = 15;

		int page = -1;

		while (System::Update()) {
			data.Update();
			if (count == 0) {
				if ((data.right.down() || data.down.down() || data.right.interval(30, 5) || data.down.interval(30, 5)) && page < data.manualStr.size() - 2) {
					count = 1;
					changeTypeFlag = false;
					data.PlayOneShot(data.aSelect);
				}
				else if ((data.left.down() || data.up.down() || data.left.interval(30, 5) || data.up.interval(30, 5)) && page > 0) {
					count = -1;
					changeTypeFlag = false;
					data.PlayOneShot(data.aSelect);
				}
				else if (data.cancel.down() || data.escape.down()) {
					count = -1;
					changeTypeFlag = true;
					data.PlayOneShot(data.aCancel);
				}
			}
			else if (count > 0) {
				if (count == transTime / 2) {
					if (changeTypeFlag) {
						page = 0;
					}
					else {
						++page;
					}
				}
				if (count == transTime) {
					count = -1;
				}
				++count;
			}
			else if (count < 0) {
				if (count == -transTime / 2) {
					if (changeTypeFlag) {
						page = -1;
					}
					else {
						--page;
					}
				}
				if (count == -transTime) {
					count = 1;
					if (changeTypeFlag)
						return;
				}
				--count;
			}

			draw();

			float c = 1;
			if (page == -1) {
				c = Pow(Sin(Math::Pi * (float)count / (float)transTime), 2);
			}

			WindowSetting::GetRect().draw(ColorF(Palette::Black).setA(0.8 * c));
			Transformer2D t(Mat3x2::Scale(static_cast<double>(WindowSetting::size.y) / 960.0));

			float y = count;
			if (y > transTime / 2)
				y -= transTime;
			if (y < -transTime / 2)
				y += transTime;
			if (page >= 0)
				data.gothic_manual(data.manualStr[page + 1]).draw(Vec2(320 + 32, 32 + y * 30), ColorF(Palette::White).setA(Pow(Cos(Math::Pi * (float)count / (float)transTime), 2)));
		}
	}

	bool WritePlayerName() {
		draw();

		int state = 0;
		Point cursor(0, 0);
		Point last(0, 0);
		String func[5] = { U"切替", U"小字", U"空白", U"戻る", U"決定" };
		String buf = data.playerName;
		int transTime = 20;
		int c = transTime;
		bool flag = false;

		while (System::Update()) {
			data.Update();
			if (c > 0) {
				--c;
			}
			else if (c < 0) {
				--c;
				if (c == -transTime) {
					return flag;
				}
			}
			else if (c == 0) {
				if (data.left.down() || data.left.interval(30, 5)) {
					cursor.x += 11;
					data.PlayOneShot(data.aSelect);
				}
				if (data.right.down() || data.right.interval(30, 5)) {
					cursor.x++;
					data.PlayOneShot(data.aSelect);
				}
				if (data.up.down() || data.up.interval(30, 5)) {
					cursor.y += 4;
					data.PlayOneShot(data.aSelect);
				}
				if (data.down.down() || data.down.interval(30, 5)) {
					cursor.y++;
					data.PlayOneShot(data.aSelect);
				}
				cursor.x %= 12;
				cursor.y %= 5;

				if (data.ok.down()) {
					if (cursor.x < 11) {
						if (state < 8 && cursor == Point(9, 4)) {
							if (buf.size() > 0) {
								auto c = SearchChar(buf.back());
								if (c.x < 8) {
									int s = c.x / 4;
									if (data.nameString[s][c.y][c.z] != data.nameString[s + 1][c.y][c.z]) {
										if (buf.back() != data.nameString[s + 1][c.y][c.z]) {
											buf.back() = data.nameString[s + 1][c.y][c.z];
										}
										else {
											buf.back() = data.nameString[s][c.y][c.z];
										}
									}
								}
							}
						}
						else if (state < 8 && cursor == Point(10, 4)) {
							if (buf.size() > 0) {
								auto c = SearchChar(buf.back());
								if (c.x < 8) {
									int s = c.x / 4;
									if (data.nameString[s][c.y][c.z] != data.nameString[s + 2][c.y][c.z]) {
										if (buf.back() != data.nameString[s + 2][c.y][c.z]) {
											buf.back() = data.nameString[s + 2][c.y][c.z];
										}
										else {
											buf.back() = data.nameString[s][c.y][c.z];
										}
									}
								}
							}
						}
						else
							buf << data.nameString[state][cursor.y][cursor.x];
						data.PlayOneShot(data.aOk);
					}
					else {
						switch (cursor.y) {
						default:
						case 0:
							state += 4;
							if (state > 8)
								state = 0;
							data.PlayOneShot(data.aOk);
							break;
						case 1:
							if (buf.size() > 0) {
								auto c = SearchChar(buf.back());
								if (c.x < 8) {
									int s = c.x / 4;
									if (data.nameString[s][c.y][c.z] != data.nameString[s + 3][c.y][c.z]) {
										if (buf.back() != data.nameString[s + 3][c.y][c.z]) {
											buf.back() = data.nameString[s + 3][c.y][c.z];
										}
										else {
											buf.back() = data.nameString[s][c.y][c.z];
										}
									}
								}
							}
							data.PlayOneShot(data.aOk);
							break;
						case 2:
							buf += ' ';
							data.PlayOneShot(data.aOk);
							break;
						case 3:
							if (buf.size() > 0)
								buf.pop_back();
							data.PlayOneShot(data.aOk);
							break;
						case 4:
							data.playerName = buf.trimmed();
							cameraman = data.playerName;
							data.PlayOneShot(data.aOk);
							c = -1;
							flag = true;
							break;
						}
					}
				}
				else if (data.cancel.down()) {
					if (buf.size() > 0)
						buf.pop_back();
					data.PlayOneShot(data.aCancel);
				}
				else if (data.escape.down()) {
					data.PlayOneShot(data.aCancel);
					c = -1;
				}
				while (buf.size() > 10) {
					buf.pop_back();
				}
			}

			draw();
			WindowSetting::GetRect().draw(ColorF(Palette::Black).setA(0.8 * Cos(Math::HalfPi * (float)c / (float)transTime)));

			Transformer2D t(Mat3x2::Translate(WindowSetting::GetRect().center()).scaled(static_cast<double>(WindowSetting::size.y) / 960.0, WindowSetting::GetRect().center()));

			data.gothic_s(U"写真を撮った人の名前を入力してください").drawBaseAt();
			data.gothic_m(buf).drawBaseAt(Point(0, 100)).bottom().draw(4);
			Size size(60, 60);
			for (auto s : step(Point(-6, 2), Point(12, 5))) {
				Rect(s * size, size).drawFrame();
				Color color = Palette::White;
				if (s.x + 6 == cursor.x && s.y - 2 == cursor.y) {
					color = Palette::Black;
					Rect(s * size, size).draw(Palette::White);
				}
				if (s.x < 5) {
					data.gothic_m(data.nameString[state][s.y - 2][s.x + 6]).drawAt(s * size + size / 2, color);
				}
				else {
					String str(U"");
					if (s.y == 2) {
						if (state == 0)
							str = U"\nカナ";
						if (state == 4)
							str = U"\nABC";
						if (state == 8)
							str = U"\nかな";
					}
					data.gothic_s(func[s.y - 2] + str).drawAt(s * size + size / 2, color);
				}
			}
		}
		return false;
	}

	Vector3D<int> SearchChar(char32 c) {
		for (auto i : step(9)) {
			for (auto s : step(Point(5, 11))) {
				if (c == data.nameString[i][s.x][s.y]) {
					return Vector3D<int>(i, s.x, s.y);
				}
			}
		}
		return Vector3D<int>();
	}
};