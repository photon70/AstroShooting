#pragma once

#include "Scene.h"

struct PhotoUnit {
	Image photo;
	Texture tex;
	int score = 0;
	const int maxRecoverScore = 30000;
	const int maxRecover = 60;

	PhotoUnit(Image photo):photo(photo) {}

	int GetRecoverScore() {
		return maxRecover * std::min(score, maxRecoverScore) / maxRecoverScore;
	}
};

struct Flash {
	const float hitMax = 0.1;
	const float cameraHitMax = 0.2;
	const float flashMax = 1;
	const float rise = hitMax / 50.f;
	const float fall = 0.02;
	float least = 0;
	float max = hitMax;
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

	void Hit(){
		if (least < max && now <= least) {
			least += rise;
			if (least > max)
				least = max;
		}
	}

	void HitCamera() {
		max = cameraHitMax;
		if (least < hitMax)
			least = hitMax;
		now = flashMax;
	}

	ColorF GetNowColor()const {
		return ColorF(now, now, now);
	}
};

class GameScene : public SceneMaster::Scene {
private:
	GameData& data;
	Texture back;
	Texture frame;

	bool cameraFlag = false;
	bool shutterFlag = false;

	int count = 0;

	int enCount = 0;

	Player player;
	Array<std::unique_ptr<Bullet>> bullet;
	std::unique_ptr<Enemy> enemy;
	Flash flash;

	Array<PhotoUnit> photo;
	int stageNum;
	double lastTime;
	int targetNum;
	int hiScore;
	int score = 0;
	int savePhoto = -1;
	bool updatePhotoFlag = false;

	int stateCnt = 0;
	const int transCnt = 20;
	int cursor = 0;
	enum State{
		Normal,
		Pause,
		Clear,
		Over
	}state = Normal;

public:
	GameScene(const InitData& init) :
		IScene(init),
		data(getData()),
		back(U"AS/back.png"),
		frame(U"AS/frame.png"),
		player(data){
		stageNum = data.GetStageNum();
		if (stageNum >= 0)
			enemy = std::make_unique<Enemy>(data.enemy[stageNum], data.script.getObjectFactory(data.enemy[stageNum].behaviorName)());
		const auto& e = data.enemy[stageNum];
		lastTime = e.timeLimit;
		targetNum = e.targetNumber;
		hiScore = data.save[stageNum].hiScore;
		bullet.reserve(2000);
	}

	~GameScene() {
		auto& s = data.save[stageNum];
		s.total += photo.size();
		data.save.Save();
	}

	void ChangeState(State s) {
		state = s;
		stateCnt = transCnt;
		int cursor = 0;
	}

	double GetProgress()const {
		return Math::Abs(static_cast<double>(stateCnt) / static_cast<double>(transCnt));
	}

	void update()override {
		ClearPrint();
		Print << bullet.size();
		if (data.updateFlag) {
			bullet.clear();
		}
		if (data.updateFlag) changeScene(U"Game");
		++count;
		switch (state)
		{
		default:
		case GameScene::Normal:
			updateNormal();
			lastTime -= System::DeltaTime();
			if (lastTime < 0) {
				lastTime = 0;
				state = Over;
			}
			if (state == Normal && !cameraFlag && !shutterFlag && data.escape.down())
				ChangeState(Pause);
			break;
		case GameScene::Pause:
			if (stateCnt > 0)--stateCnt;
			if (stateCnt < 0)--stateCnt;
			if (stateCnt == -transCnt)
				ChangeState(Normal);
			if (stateCnt == 0) {
				if (data.up.down())
					cursor += 3;
				if (data.down.down())
					cursor++;
				cursor %= 4;
				if (data.escape.down()) {
					stateCnt = -1;
				}
				if (data.ok.down()) {
					switch (cursor)
					{
					default:
					case 0:
						stateCnt = -1;
						break;
					case 1:
						data.isPageForward = false;
						changeScene(U"Menu");
						break;
					case 2:
						break;
					case 3:
						changeScene(U"Game");
						break;
					}
				}
			}
			break;
		case GameScene::Clear:
			if (stateCnt > 0)--stateCnt;
			if (stateCnt < 0)--stateCnt;
			if (stateCnt == -transCnt)
				ChangeState(Normal);
			if (stateCnt == 0) {
				if (updatePhotoFlag) {
					if (data.up.down())
						cursor += 3;
					if (data.down.down())
						cursor++;
					cursor %= 4;
				}
				else {
					if (data.up.down())
						cursor += 2;
					if (data.down.down())
						cursor++;
					cursor %= 3;
				}

				if (data.left.down())
					savePhoto+= photo.size() - 1;
				if (data.right.down())
					savePhoto++;
				savePhoto %= photo.size();
				if (data.ok.down()) {
					switch (cursor)
					{
					default:
					case 0:
						data.GoNextStage();
						changeScene(U"Game");
						break;
					case 1:
						data.isPageForward = false;
						changeScene(U"Menu");
						break;
					case 2:
						changeScene(U"Game");
						break;
					case 3:
						SavePhoto(savePhoto);
						data.save.Save();
						updatePhotoFlag = false;
						break;
					}
				}
			}
			break;
		case GameScene::Over:
			if (stateCnt > 0)--stateCnt;
			if (stateCnt < 0)--stateCnt;
			if (stateCnt == 0) {
				if (data.up.down())
					cursor += 3;
				if (data.down.down())
					cursor++;
				cursor %= 4;
				if (data.ok.down()) {
					switch (cursor)
					{
					default:
					case 0:
						changeScene(U"Game");
						break;
					case 1:
						data.isPageForward = false;
						changeScene(U"Menu");
						break;
					case 2:
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
			photo << PhotoUnit(ScreenCapture::GetFrame().clipped(Rect(Arg::center = Window::Center(), 480, 480)));
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
			for (auto& b : bullet) {
				if (b && q.intersects(b->data.pos)) {
					p.score += b->container.score;
					b->data.deleteFlag = true;
				}
			}
			if (q.intersects(enemy->data.pos)) {
				score += p.score;
					if (savePhoto == -1 || photo[savePhoto].score <= photo.back().score) {
						savePhoto = photo.size() - 1;
					}
				if (hiScore < score) {
					hiScore = score;
						updatePhotoFlag = true;
				}

				if (photo.size() == targetNum) {
					state = Clear;
					auto& s = data.save[stageNum];
					s.hiScore = hiScore;
					if (updatePhotoFlag)
						SavePhoto(savePhoto);

					data.save.Save();
				}
				player.SetCameraInterval(p.GetRecoverScore() / 2);
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
		inter.cameraFlag = cameraFlag;

		if (!cameraFlag) {
			player.Update(inter);
		}
		else {
			player.MoveCamera(inter);
		}
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
						}
						else {
							flash.Hit();
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

		RegisterUnit/*<Bullet>*/(bullet, inter.bulletBuf);

		cameraFlag = inter.cameraFlag;
		shutterFlag = inter.shutterFlag;
		if (shutterFlag) {
			ScreenCapture::RequestCurrentFrame();
		}
	}

	void draw()const override {
		if (!shutterFlag) {
			Transformer2D t(Mat3x2::Translate(Window::Center()).scaled(static_cast<double>(Window::Height()) / 960.0, Window::Center()));

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
						b->Draw();
				}
			}
			data.field.draw(ColorF(Palette::White).setA(flash.now));

			if (cameraFlag)
				player.DrawCamera();

			data.gothic_l(U"{:.1f}"_fmt(lastTime)).drawAt(Vec2(0, -415).movedBy(1.5, 2), Palette::Dimgray);
			data.gothic_l(U"{:.1f}"_fmt(lastTime)).drawAt(Vec2(0, -415));

			if (state == Pause || state == Clear || state == Over) {
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
					choice[0] = U"つぎのステージへすすむ";
					choice[1] = U"タイトルにもどる";
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
				}
				data.mk_pause_title(title).draw(Vec2(-300 + GetProgress() * 100, 120), ColorF(Palette::White).setA(1 - GetProgress()));
				for (auto i : step(4)) {
					ColorF c = i == cursor ? Palette::White : Palette::Gray;
					if (state == Clear && i == 3 && !updatePhotoFlag)
						c *= 0.3;
					c.setA(1 - GetProgress());
					data.gothic_pause(choice[i]).draw(Vec2(-260 + GetProgress() * 100, 200 + 50 * i), c);
				}
			}
			if (state == Clear) {
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

			for (auto i : step(10)) {
				if (i < photo.size()) {
					auto v = Point(i < 5 ? -500 : 500, 180 * i - 360);
					double scale = 0.5;
					photo[i].tex.scaled(scale).drawAt(v);
					ColorF c = i == savePhoto ? Palette::Pink : Palette::Lightgreen;
					data.gothic_s(photo[i].score).draw(Arg::bottomRight = v + Point(100, 80).movedBy(1.7, 1.2), Palette::Black);
					data.gothic_s(photo[i].score).draw(Arg::bottomRight = v + Point(100, 80), c);
				}
			}
			if (state == Clear) {
				auto v = Point(savePhoto < 5 ? -500 : 500, 180 * savePhoto - 360);
				double scale = 0.7;
				photo[savePhoto].tex.scaled(scale).drawAt(v, ColorF(1.0, 1.0, pow(sin(ToRadians(count) * 5), 2)));
				ColorF c = Palette::Pink;
				data.gothic_s(photo[savePhoto].score).draw(Arg::bottomRight = v + Point(100, 80).movedBy(1.7, 1.2), Palette::Black);
				data.gothic_s(photo[savePhoto].score).draw(Arg::bottomRight = v + Point(100, 80), c);
			}
		}
		else {
			Window::ClientRect().draw(flash.GetNowColor());
			Transformer2D t(Mat3x2::Translate(Window::Center() - player.GetFinderPos()));

			back.drawAt(Float2::Zero());
			//data.field.draw(flash.GetNowColor());
			{
			//	RenderStateBlock2D blend(BlendState::Additive);

				enemy->Draw(data.script);

				for (auto& b : bullet) {
					if (b && b->data.count > 0)
						b->Draw();
				}
			}
			data.field.draw(ColorF(Palette::White).setA(flash.now));
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
		for(;fromIt != fromItEnd; ++fromIt){
			to.emplace_back(fromIt->release());
		}
		Print << enCount;

		//Arrayで管理。先頭から消去するかを判断していって、登録するものがあるならswapないなら中身を消す。
	}

	void SavePhoto(int num, String name = U"") {
		auto& s = data.save[stageNum];
		auto& p = photo[num];
		s.score = p.score;
		s.cameraman = name;
		p.photo.savePNG(U"AS/stage{}.png"_fmt(stageNum + 1));
	}
};