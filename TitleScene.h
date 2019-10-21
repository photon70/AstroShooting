#pragma once

#include "Scene.h"

void ChangeWindowSize(int type) {

}

void ChangeBGMVolume(int rate) {

}

void ChangeSoundVolume(int rate) {

}

class TitleScene : public SceneMaster::Scene {
private:
	const Vec2 drawPosHome = Vec2(0, -320);
	const Vec2 drawPosToMenu = Vec2(0, 0);
	const Vec2 drawPosToManual = Vec2(1280, -0);
	const int transTime = 20;

	Texture title = Texture(GetResource(U"AS/title.png"));
	GameData& data;
	int select = 0;
	int count = 0;

	Vec2 drawPos = drawPosHome;
	double scale = 1.0;
	float stringDence = 1;

	enum {
		Normal,
		ToMenu,
		ToManual,
		Settings,
		FromMenu,
	}state = Normal;

public:
	TitleScene(const InitData& init) :
		IScene(init),
		data(getData()){
		Graphics::SetBackground(data.backColor);

		if (!data.isPageForward) {
			count = transTime;
			drawPos = drawPosToMenu;
			scale = 0.5;
			stringDence = 0;
			state = FromMenu;
		}
		data.PlayBGM(data.aTitle);
	}

	void update()override {
		double t = 0;
		switch (state)
		{
		default:
		case TitleScene::Normal:
			if (data.up.down()) {
				select += 3;
				count = 0;
				data.PlayOneShot(data.aSelect);
			}
			if (data.down.down()) {
				++select;
				count = 0;
				data.PlayOneShot(data.aSelect);
			}
			select %= 4;

			++count;

			if (data.ok.down()) {
				count = 0;
				switch (select)
				{
				default:
				case 0:
					state = ToMenu;
					break;
				case 1:
					state = ToManual;
					break;
				case 2:
					state = Settings;
					break;
				case 3:
					System::Exit();
					break;
				}
				data.PlayOneShot(data.aOk);
			}
			break;
		case TitleScene::ToMenu:
			if (count < transTime) {
				++count;
			}
			else {
				data.isPageForward = true;
				data.PlayOneShot(data.aPage);
				changeScene(U"Menu", 500, true);
			}
			t = EaseInSine(static_cast<double>(count) / static_cast<double>(transTime));
			drawPos = drawPosHome + (drawPosToMenu - drawPosHome) * t;
			scale = 1.0 - t / 2;
			stringDence = 1.0 - t;
			break;
		case TitleScene::ToManual:
			if (count < transTime) {
				++count;
			}
			else {
				data.isPageForward = true;
				data.PlayOneShot(data.aPage);
				changeScene(U"Manual", 500, true);
			}
			t = EaseInSine(static_cast<double>(count) / static_cast<double>(transTime));
			drawPos = drawPosHome + (drawPosToManual - drawPosHome) * t;
			scale = 1.0 - t / 2;
			stringDence = 1.0 - t;
			break;
		case TitleScene::Settings:
			if (count < transTime) {
				++count;
			}else{
				data.isPageForward = true;
				changeScene(U"Setting", 500, true);
			}
			t = EaseInSine(static_cast<double>(count) / static_cast<double>(transTime));
			stringDence = 1.0 - t;
			break;
		case TitleScene::FromMenu:
			if (0 < count) {
				--count;
			}
			else {
				state = TitleScene::Normal;
			}
			t = EaseInSine(static_cast<double>(count) / static_cast<double>(transTime));
			drawPos = drawPosHome + (drawPosToMenu - drawPosHome) * t;
			scale = 1.0 - t / 2;
			stringDence = 1.0 - t;
			break;
		}
	}

	void draw()const override {
		Transformer2D t(Mat3x2::Scale(scale * static_cast<double>(WindowSetting::size.y) / 960.0));

		title.draw(drawPos);
		
		auto d = drawPos - drawPosHome;
		String str[4] = { U"Play!_‚ ‚»‚Ô" , U"Manual_‚ ‚»‚Ñ‚©‚½", U"Settings_‚¹‚Á‚Ä‚¢", U"Quit_‚¨‚í‚é"};
		ColorF color[4] = { ColorF(1.0, 0.2, 0.1), Palette::Orange, Palette::Yellow, Palette::Aqua };
		for (auto s : step(4)) {
			auto vec = Vec2(600, 600 + s * 80) + d;
			if (s == select && state == TitleScene::Normal)
				vec.x -= 30 * Sin(180_deg * Min(1.f, (float)count / (float)transTime));
			data.mk_title(str[s]).draw(vec, ColorF(select != s ? Palette::Lightgrey : color[s]).setA(stringDence));
		}
	}

	void drawFadeIn(double t)const override {
		if (data.isPageForward) {
			SceneMaster::Scene::drawFadeIn(t);
		}
		else {
			RasterizerState rasterizer = RasterizerState::Default2D;
			rasterizer.scissorEnable = true;

			RenderStateBlock2D r(rasterizer);

			auto w = WindowSetting::GetSize();
			float hw = w.x / 2;

			if (t > 0.5) {
				Graphics2D::SetScissorRect(Rect(hw, 0, hw, w.y));
				WindowSetting::GetRect().draw(ColorF(data.backColor).setA(1.0 - (t - 0.5) * 2.0));
			}
			Graphics2D::SetScissorRect(WindowSetting::GetRect());

			Transformer2D trans(Mat3x2::Scale(t, 1.0));
			draw();
		}
	}

	void drawFadeOut(double t)const override {
		Transformer2D trans;
		switch (state){
		case TitleScene::Normal:
		case TitleScene::Settings:
		default:
			SceneMaster::Scene::drawFadeOut(t);
			break;
		case TitleScene::ToMenu:
			if(t < 0.5)
				WindowSetting::GetRect().draw(ColorF(data.backColor).setA(t * 2));

			trans = Transformer2D(Mat3x2::Scale(1.0 - t, 1.0));
			draw();
			break;
		case TitleScene::ToManual:
			if (t < 0.5) {
				trans = Transformer2D(Mat3x2::Scale(1.0 - t * 2, 1.0, Float2(WindowSetting::size.x / 2, 0)));
				draw();
			}
			break;
		}
	}
};