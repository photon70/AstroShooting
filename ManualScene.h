#pragma once

#include "Scene.h"

class ManualScene : public SceneMaster::Scene {
private:
	GameData& data;

	Texture manual = Texture(GetResource(U"AS/manual.png"));//TODO
	Texture circle = Texture(GetResource(U"AS/circle.png"));
	Vec2 drawPos = Vec2::Zero();
	int page;

	int toTitleCount = 0;
	const int transTime = 20;

public:
	ManualScene(const InitData& init) :
		IScene(init),
		data(getData()){
		page = data.manualPage;
	}

	void update()override {
		if (toTitleCount == 0) {
			if ((data.right.down() || data.right.interval(30, 5)) && page < data.manualStr.size() / 2 - 1) {
				data.manualPage++;
				data.isPageForward = true;
				data.PlayOneShot(data.aPage);
				changeScene(U"Manual", 300, true);
			}
			else if ((data.left.down() || data.left.interval(30, 5)) && page > 0) {
				data.manualPage--;
				data.isPageForward = false;
				data.PlayOneShot(data.aPage);
				changeScene(U"Manual", 300, true);
			}
			else if (data.left.down() || data.escape.down()) {
				toTitleCount = 1;
			}
			else if (page == 0) {
				if (data.down.down() || data.down.interval(30, 5)) {
					++data.manualSelect ;
					data.PlayOneShot(data.aSelect);
				}
				else if (data.up.down() || data.up.interval(30, 5)) {
					data.manualSelect  += 7;
					data.PlayOneShot(data.aSelect);
				}
				else if (data.ok.down()) {
					data.manualPage = 1 + data.manualSelect  / 2;
					data.isPageForward = true;
					data.PlayOneShot(data.aPage);
					changeScene(U"Manual", 300, true);
				}
				else if (data.cancel.down()) {
					toTitleCount = 1;
				}
				data.manualSelect  %= 8;
			}
			else {
				if (data.ok.down() && page < data.manualStr.size() / 2 - 1) {
					data.manualPage++;
					data.isPageForward = true;
					data.PlayOneShot(data.aPage);
					changeScene(U"Manual", 300, true);
				}
				else if (data.cancel.down()) {
					data.manualPage = 0;
					data.isPageForward = false;
					data.PlayOneShot(data.aPage);
					changeScene(U"Manual", 300, true);
				}
			}
		}
		else {
			if (toTitleCount == transTime) {
				data.manualPage = 0;
				data.isPageForward = false;
				data.PlayOneShot(data.aPage);
				changeScene(U"Title", 300, true);
			}
			if(toTitleCount < transTime)
				++toTitleCount;
			drawPos.x = -640.0 * (float)(toTitleCount) / (float)(transTime);
		}
	}

	void draw()const override {
		Transformer2D t(Mat3x2::Scale(static_cast<double>(WindowSetting::size.y) / 960.0));

		manual.draw(drawPos);

		if (page == 0) {
			data.mk_menu_s(data.manualStr[0]).draw(drawPos + Point(32, 32), Palette::Black);
		}
		else {
			data.gothic_manual(data.manualStr[page * 2]).draw(drawPos + Point(32, 32), Palette::Black);
		}
		data.gothic_manual(data.manualStr[page * 2 + 1]).draw(drawPos + Point(32 + 640, 32), Palette::Black);
		
		if (page == 0) {
			circle.drawAt(drawPos + Point(640 + 84, 148 + data.manualSelect  * 92));
		}
	}

	void drawFadeIn(double t)const override {
		if (data.isPageForward) {
			
			RasterizerState rasterizer = RasterizerState::Default2D;
			rasterizer.scissorEnable = true;

			RenderStateBlock2D r(rasterizer);

			auto w = WindowSetting::GetSize();
			float hw = w.x / 2;

			if (t < 0.5) {
				Graphics2D::SetScissorRect(Rect(hw + hw * (1.0 - 2 * t), 0, hw, w.y));
			}
			else {
				Graphics2D::SetScissorRect(Rect(hw, 0, w.x, w.y));
			}
			draw();

			if (t >= 0.5) {
				Graphics2D::SetScissorRect(Rect(hw * (1.0 - 2 * (t - 0.5)), 0, hw * 2 * (t - 0.5), w.y));
			
				Transformer2D trans = Transformer2D(Mat3x2::Scale(2 * (t - 0.5), 1.0, Float2(hw, 0)));

				draw();
			}
		}
		else {
			t = 1.0 - t;

			RasterizerState rasterizer = RasterizerState::Default2D;
			rasterizer.scissorEnable = true;

			RenderStateBlock2D r(rasterizer);

			auto w = WindowSetting::GetSize();
			float hw = w.x / 2;

			if (t < 0.5) {
				Graphics2D::SetScissorRect(Rect(hw, 0, hw * (1.0 - 2 * t), w.y));

				Transformer2D trans = Transformer2D(Mat3x2::Scale(1.0 - 2 * t, 1.0, Float2(hw, 0)));

				draw();
			}

			if (t < 0.5) {
				Graphics2D::SetScissorRect(Rect(0, 0, hw, w.y));
			}
			else {
				Graphics2D::SetScissorRect(Rect(0, 0, hw * (1.0 - 2 * (t - 0.5)), w.y));
			}

			draw();
		}
	}

	void drawFadeOut(double t)const override {
		if (toTitleCount > 0) {
			draw();
		}
		else {
			if (data.isPageForward) {

				RasterizerState rasterizer = RasterizerState::Default2D;
				rasterizer.scissorEnable = true;

				RenderStateBlock2D r(rasterizer);

				auto w = WindowSetting::GetSize();
				float hw = w.x / 2;

				if (t < 0.5) {
					Graphics2D::SetScissorRect(Rect(hw, 0, hw * (1.0 - 2 * t), w.y));

					Transformer2D trans = Transformer2D(Mat3x2::Scale(1.0 - 2 * t, 1.0, Float2(hw, 0)));

					draw();
				}

				if (t < 0.5) {
					Graphics2D::SetScissorRect(Rect(0, 0, hw, w.y));
				}
				else {
					Graphics2D::SetScissorRect(Rect(0, 0, hw * (1.0 - 2 * (t - 0.5)), w.y));
				}

				draw();
			}
			else {
				t = 1.0 - t;

				RasterizerState rasterizer = RasterizerState::Default2D;
				rasterizer.scissorEnable = true;

				RenderStateBlock2D r(rasterizer);

				auto w = WindowSetting::GetSize();
				float hw = w.x / 2;

				if (t < 0.5) {
					Graphics2D::SetScissorRect(Rect(hw + hw * (1.0 - 2 * t), 0, hw, w.y));
				}
				else {
					Graphics2D::SetScissorRect(Rect(hw, 0, w.x, w.y));
				}
				draw();

				if (t >= 0.5) {
					Graphics2D::SetScissorRect(Rect(hw * (1.0 - 2 * (t - 0.5)), 0, hw * 2 * (t - 0.5), w.y));

					Transformer2D trans = Transformer2D(Mat3x2::Scale(2 * (t - 0.5), 1.0, Float2(hw, 0)));

					draw();
				}
			}
		}
	}
};