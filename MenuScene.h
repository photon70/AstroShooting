#pragma once

#include "Scene.h"

class MenuScene : public SceneMaster::Scene {
private:
	Texture menu = Texture(GetResource(U"AS/menu.png"));
	Texture cursor = Texture(GetResource(U"AS/cursor.png"));
	Texture circle = Texture(GetResource(U"AS/circle.png"));
	Texture player = Texture(GetResource(U"AS/player_as.png"));
	Texture tape = Texture(GetResource(U"AS/tape.png"));
	GameData& data;

	Array<Texture> photo;

	int page = 0;
	int select = 0;

public:
	MenuScene(const InitData& init) :
		IScene(init),
		data(getData()) {
		Graphics::SetBackground(data.backColor);
		page = data.page;
		select = data.select;
		for (auto i : step(data.enemy.size())) {
			photo << Texture(U"AS/stage{}.png"_fmt(i + 1));
		}
		data.maxPage = data.pageStr.size();
		data.PlayBGM(data.aTitle);
	}

	void update()override {
		if (data.CheckPageAvailable()) {

		}
		if (data.pageFlag) {
			if (data.left.down()) {
				if (page > 0) {
					--data.page;
					data.select = select;
					data.isPageForward = false;
					data.PlayOneShot(data.aPage);
					changeScene(U"Menu", 300, true);
				}
			}
			if (data.right.down()) {
				if (page < data.maxPage - 1 && page <= data.availablePage) {
					++data.page;
					data.select = select;
					data.isPageForward = true;
					data.PlayOneShot(data.aPage);
					changeScene(U"Menu", 300, true);
				}
			}
			if (data.up.down()) {
				select = 4 + select % 2;
				data.pageFlag = false;
				data.PlayOneShot(data.aSelect);
			}
			if (data.down.down()) {
				select = select % 2;
				data.pageFlag = false;
				data.PlayOneShot(data.aSelect);
			}
			if (data.cancel.down()) {
				data.select = select;

				data.isPageForward = false;
				data.PlayOneShot(data.aCancel);
				data.PlayOneShot(data.aPage);
				changeScene(U"Title", 300, true);
			}
		}
		else {
			if (data.left.down()) {
				if (select % 2 == 1) {
					select--;
					data.PlayOneShot(data.aSelect);
				}
				else if (page > 0) {
					--data.page;
					data.select = select + 1;
					data.isPageForward = false;
					data.PlayOneShot(data.aPage);
					changeScene(U"Menu", 300, true);
				}
			}
			if (data.right.down()) {
				if (select % 2 == 0) {
					select++;
					data.PlayOneShot(data.aSelect);
				}
				else if (page < data.maxPage - 1 && page <= data.availablePage) {
					++data.page;
					data.select = select - 1;
					data.isPageForward = true;
					data.PlayOneShot(data.aPage);
					changeScene(U"Menu", 300, true);
				}
			}
			if (data.up.down()) {
				if (select > 1)
					select -= 2;
				else {
					data.pageFlag = true;
				}
				data.PlayOneShot(data.aSelect);
			}
			if (data.down.down()) {
				if (select < 4)
					select += 2;
				else {
					data.pageFlag = true;
				}
				data.PlayOneShot(data.aSelect);
			}
			if (data.cancel.down() || data.escape.down()) {
				data.select = select;

				data.isPageForward = false;
				data.PlayOneShot(data.aCancel);
				data.PlayOneShot(data.aPage);
				changeScene(U"Title", 300, true);
			}
			if (data.ok.down() && GetStageNum() >= 0) {
				if (page <= data.availablePage) {
					data.select = select;
					data.isPageForward = true;
					data.PlayOneShot(data.aOk);
					changeScene(U"Game", 600, false);
				}
				else {
					data.PlayOneShot(data.aCancel);
				}
			}
		}
	}

	void draw()const override {
		Transformer2D t(Mat3x2::Scale(static_cast<double>(WindowSetting::size.y) / 960.0));

		menu.draw();

		for(auto s : step(Size(2, 3))){
			DrawStage(s);
		}

		if (data.pageFlag) {
			circle.draw(Arg::topCenter = Point(320, 0));
		}else
			cursor.draw((float)320 * (select % 2), (float)304 * (select / 2) + 32);

		int n = GetStageNum();
		data.mk_menu(page + 1).draw(Arg::topCenter = Point(320, 10), Palette::Black);
		if(page <= data.availablePage)
			data.mk_menu(data.pageStr[page]).draw(660, 10);
		if (page > 0)
			data.mk_menu(U"Å©").draw(Arg::topCenter = Point(280, 10), Palette::Black);
		if (page < data.maxPage - 1 && page <= data.availablePage)
			data.mk_menu(U"Å®").draw(Arg::topCenter = Point(360, 10), Palette::Black);
		player.drawAt(690, 840);
		Rect(740, 770, 500, 150).draw(Palette::White);
		data.mk_menu_s(U"Å™Å´Å©Å®Ç≈Ç§Ç≤Ç©Ç∑Å@ZÉLÅ[Ç≈Ç¶ÇÁÇ‘").draw(Arg::bottomCenter = Point(960, 960));
		if (!data.pageFlag && n < data.enemy.size()) {
			const auto &e = data.enemy[n];
			const auto& s = data.save[n];
			String attach;
			switch (e.stageType) {
			default:
			case Enemy::Normal:
				break;
			case Enemy::SuddenDeath:
				attach = U"àÍìxÇ‡íeÇ…ìñÇΩÇÁÇ∏Ç…\n";
				break;
			case Enemy::Tutorial:
				attach = U"ó˚èKÇÃÇΩÇﬂÇ…\n";
				break;
			}
			Rect r(720, 128, 480, 480);
			r.draw(Palette::Lightgrey);
			if (s.total > 0) {
				data.mk_menu_s(U"éBâeñáêî").draw(660, 720);
				data.mk_menu_s(s.total).draw(Arg::topRight = Point(930, 720));
			}
			if (data.save[n].hiScore > 0) {
				photo[n].draw(r.pos);
				data.mk_menu(U"{}{}Ç{}ñáéBÇÎÇ§ÅI"_fmt(attach, e.nickname, e.targetNumber)).draw(Arg::bottomCenter = r.topCenter());
				data.mk_menu_l(e.name).draw(Arg::topCenter = r.bottomCenter());
				data.mk_menu_s(U"ç≈çÇìæì_").draw(660, 680);
				data.mk_menu_s(U"éBâeé“").draw(950, 680);
				data.mk_menu_s(U"ìæì_").draw(950, 720);
				data.mk_menu_s(s.hiScore).draw(Arg::topRight = Point(930, 680));
				data.mk_menu_s(s.cameraman).draw(Arg::topRight = Point(1260, 680));
				data.mk_menu_s(s.score).draw(Arg::topRight = Point(1260, 720));

				data.mk_menu_txt(e.txt).draw(750, 775, Palette::Black);

			}
			else if(page <= data.availablePage){
				data.mk_menu(U"{}{}Ç\n{}ñáéBÇÎÇ§ÅI"_fmt(attach, e.nickname, e.targetNumber)).drawAt(r.center(), Palette::Black);
				data.mk_menu_txt(data.pageBrief[page]).draw(750, 775, Palette::Black);
			}
			else {
				data.mk_menu(U"ìVëÃé ê^Ççáåv{}éÌóﬁ\néBÇÈÇ∆óVÇ◊ÇÈÇÊÇ§Ç…Ç»ÇÈÇÊ"_fmt(data.pageLock[page])).drawAt(r.center(), Palette::Black);
				data.mk_menu_txt(U"Ç±ÇÃï”ÇËÇ©ÇÁÅAåıÇÃíeÇÕ\nîÚÇÒÇ≈Ç±Ç»Ç¢ÇÀÅB").draw(750, 775, Palette::Black);
			}
		}
		else {
			Rect r(720, 128, 480, 480);
			r.draw(Palette::Lightgrey);
			if (page <= data.availablePage) {
				data.mk_menu_txt(data.pageBrief[page]).draw(750, 775, Palette::Black);
			}
			else {
				data.mk_menu(U"ìVëÃé ê^Ççáåv{}ñá\néBÇÈÇ∆óVÇ◊ÇÈÇÊÇ§Ç…Ç»ÇÈÇÊ"_fmt(data.pageLock[page])).drawAt(r.center(), Palette::Black);
				data.mk_menu_txt(U"Ç±ÇÃï”ÇËÇ©ÇÁÅAåıÇÃíeÇÕ\nîÚÇÒÇ≈Ç±Ç»Ç¢ÇÀÅB").draw(750, 775, Palette::Black);
			}
		}
	}

	void DrawStage(Point s)const {
		auto n = 6 * page + s.y * 2 + s.x;
		if (n < data.enemy.size()) {
			if (data.save[n].hiScore > 0) {
				photo[n].scaled(256.0 / 480.0).draw(Point(320 * s.x, 304 * s.y + 32) + Point(32, 16));
				data.mk_menu_stageTitle(data.enemy[n].name).drawAt(Point(320 * s.x, 304 * s.y + 32) + Point(160, 295), Palette::Black);
			}
			else if(page <= data.availablePage){
				tape.drawAt(Point(320 * s.x, 304 * s.y + 32) + Point(160, 144));
			}
		}
	}

	//ëIëÇ≥ÇÍÇƒÇ¢ÇÈÉXÉeÅ[ÉWî‘çÜÅ@ë∂ç›ÇµÇ»Ç¢Ç∆Ç´ÇÕ-1
	int GetStageNum()const {
		auto n = page * 6 + select;
		if (n >= data.enemy.size())return -1;
		return n;
	}

	void drawFadeIn(double t)const override {
		RasterizerState rasterizer = RasterizerState::Default2D;
		rasterizer.scissorEnable = true;

		RenderStateBlock2D r(rasterizer);

		auto w = WindowSetting::GetSize();
		float hw = w.x / 2;

		if (data.isPageForward) {
			Graphics2D::SetScissorRect(Rect(static_cast<int>(hw * (1.0 - t)), 0, hw * t, w.y));
			draw();

			Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
			if (t > 0.5) {
				draw();
				WindowSetting::GetRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
			}
		}
		else {

			Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
			if (t > 0.5) {
				draw();
				WindowSetting::GetRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
			}
			Graphics2D::SetScissorRect(Rect(0, 0, hw * t, w.y));

			Transformer2D trans(Mat3x2::Scale(t, 1.0));
			draw();

		}
	}

	void drawFadeOut(double t)const override {
		RasterizerState rasterizer = RasterizerState::Default2D;
		rasterizer.scissorEnable = true;

		RenderStateBlock2D r(rasterizer);

		t = 1.0 - t;
		auto w = WindowSetting::GetSize();
		float hw = w.x / 2;

		if (!data.isPageForward) {
			Graphics2D::SetScissorRect(Rect(static_cast<int>(hw * (1.0 - t)), 0, hw * t, w.y));
			draw();

			Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
			if (t > 0.5) {
				draw();
				WindowSetting::GetRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
			}
		}
		else {

			Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
			if (t > 0.5) {
				draw();
				WindowSetting::GetRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
			}
			Graphics2D::SetScissorRect(Rect(0, 0, hw * t, w.y));

			Transformer2D trans(Mat3x2::Scale(t, 1.0));
			draw();

		}
	}
};