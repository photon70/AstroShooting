#pragma once

#include "Scene.h"

class MenuScene : public SceneMaster::Scene {
private:
	Texture menu = Texture(U"AS/menu.png");
	Texture cursor = Texture(U"AS/cursor.png");
	Texture player = Texture(U"AS/player_as.png");
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
	}

	void update()override {
		if (data.left.down()) {
			if (select % 2 == 1) {
				select--;
			}
			else if (page > 0) {
				--data.page;
				data.select = select + 1;
				data.isPageForward = false;
				changeScene(U"Menu", 300, true);
			}
		}
		if (data.right.down()) {
			if (select % 2 == 0) {
				select++;
			}
			else if(page < data.maxPage - 1){
				++data.page;
				data.select = select - 1;
				data.isPageForward = true;
				changeScene(U"Menu", 300, true);
			}
		}
		if (data.up.down()) {
			if (select > 1)
				select -= 2;
			else select += 4;
		}
		if (data.down.down()) {
			if (select < 4)
				select += 2;
			else select -= 4;
		}
		if (data.cancel.down()) {
			data.select = select;

			data.isPageForward = false;
			changeScene(U"Title", 300, true);
		}
		if (data.ok.down() && GetStageNum() >= 0) {
			data.select = select;
			data.isPageForward = true;
			changeScene(U"Game", 600, false);
		}
	}

	void draw()const override {
		Transformer2D t(Mat3x2::Scale(static_cast<double>(Window::Height()) / 960.0));

		menu.draw();

		for(auto s : step(Size(2, 3))){
			DrawStage(s);
		}

		cursor.draw((float)320 * (select % 2), (float)320 * (select / 2));

		int n = GetStageNum();
		data.mk_menu(U"ページ").draw(660, 10);
		data.mk_menu(page + 1).draw(Arg::topCenter = Point(860, 10));
		data.mk_menu(data.pageStr[page]).draw(960, 10);
		if (page > 0)
			data.mk_menu(U"←").draw(Arg::topCenter = Point(820, 10));
		if (page < data.maxPage - 1)
			data.mk_menu(U"→").draw(Arg::topCenter = Point(900, 10));
		player.drawAt(690, 840);
		Rect(740, 770, 500, 150).draw(Palette::White);
		data.mk_menu_s(U"↑↓←→でうごかす　Zキーでえらぶ").draw(Arg::bottomCenter = Point(960, 960));
		if (n < data.enemy.size()) {
			const auto &e = data.enemy[n];
			const auto& s = data.save[n];
			Rect r(720, 128, 480, 480);
			r.draw(Palette::Lightgrey);
			if (s.total > 0) {
				data.mk_menu_s(U"撮影枚数").draw(660, 720);
				data.mk_menu_s(s.total).draw(Arg::topRight = Point(930, 720));
			}
			if (photo[n]) {
				photo[n].draw(r.pos);
				data.mk_menu(U"{}を{}枚撮ろう！"_fmt(e.nickname, e.targetNumber)).draw(Arg::bottomCenter = r.topCenter());
				data.mk_menu_l(e.name).draw(Arg::topCenter = r.bottomCenter());
				data.mk_menu_s(U"最高得点").draw(660, 680);
				data.mk_menu_s(U"撮影者").draw(950, 680);
				data.mk_menu_s(U"得点").draw(950, 720);
				data.mk_menu_s(s.hiScore).draw(Arg::topRight = Point(930, 680));
				data.mk_menu_s(s.cameraman).draw(Arg::topRight = Point(1260, 680));
				data.mk_menu_s(s.score).draw(Arg::topRight = Point(1260, 720));

				data.mk_menu_txt(e.txt).draw(750, 775, Palette::Black);

			}
			else {
				data.mk_menu(U"{}を\n{}枚撮ろう！"_fmt(e.nickname, e.targetNumber)).drawAt(r.center(), Palette::Black);
			}
		}
	}

	void DrawStage(Point s)const {
		auto n = 6 * page + s.y * 2 + s.x;
		if (n < data.enemy.size()) {
			if (photo[n]) {
				photo[n].scaled(256.0 / 480.0).draw(320 * s + Point(32, 16));
				data.mk_menu_stageTitle(data.enemy[n].name).drawAt(320 * s + Float2(160, 300), Palette::Black);
			}
		}
	}

	//選択されているステージ番号　存在しないときは-1
	int GetStageNum()const {
		auto n = page * 6 + select;
		if (n >= data.enemy.size())return -1;
		return n;
	}

	void drawFadeIn(double t)const override {
		RasterizerState rasterizer = RasterizerState::Default2D;
		rasterizer.scissorEnable = true;

		RenderStateBlock2D r(rasterizer);

		auto w = Window::Size();
		float hw = w.x / 2;

		if (data.isPageForward) {
			Graphics2D::SetScissorRect(Rect(static_cast<int>(hw * (1.0 - t)), 0, hw * t, w.y));
			draw();

			Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
			if (t > 0.5) {
				draw();
				Window::ClientRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
			}
		}
		else {

			Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
			if (t > 0.5) {
				draw();
				Window::ClientRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
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
		auto w = Window::Size();
		float hw = w.x / 2;

		if (!data.isPageForward) {
			Graphics2D::SetScissorRect(Rect(static_cast<int>(hw * (1.0 - t)), 0, hw * t, w.y));
			draw();

			Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
			if (t > 0.5) {
				draw();
				Window::ClientRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
			}
		}
		else {

			Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
			if (t > 0.5) {
				draw();
				Window::ClientRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
			}
			Graphics2D::SetScissorRect(Rect(0, 0, hw * t, w.y));

			Transformer2D trans(Mat3x2::Scale(t, 1.0));
			draw();

		}
	}
};