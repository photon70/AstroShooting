#include "Scene.h"
#include "TitleScene.h"
#include "SettingScene.h"
#include "MenuScene.h"
#include "ManualScene.h"
#include "GameScene.h"

#include "ScriptBind.h"

Size WindowSetting::size;

void Main()
{
	//WindowSetting::size = Size(1280, 960);
	//if (true) {
	//	Window::Resize(WindowSetting::GetSize());
	//}
	//else {
	//	Window::SetBaseSize(WindowSetting::GetSize());

	//	const auto [displayIndex, displayMode]
	//		= OptimalScreen::Get(OptimalScreen::Preference::AspectMin, WindowSetting::GetSize());

	//	Graphics::SetFullScreen(true, displayMode.size, displayIndex, displayMode.refreshRateHz);

	////	Print << U"Display {} | size {} @ {} Hz"_fmt(displayIndex, displayMode.size, displayMode.refreshRateHz);
	//}

	System::SetExitEvent(WindowEvent::CloseButton);
	Window::SetTitle(U"AstroShooting");

	ph::RegisterYield();
	RegisterTypes();

	SceneMaster scene;
	scene.setFadeColor(Palette::Black);
	scene.add<TitleScene>(U"Title");
	scene.add<SettingScene>(U"Setting");
	scene.add<MenuScene>(U"Menu");
	scene.add<ManualScene>(U"Manual");
	scene.add<GameScene>(U"Game");
	
	Graphics::SetBackground(scene.get()->backColor);

	scene.get()->Reload();

	scene.drawScene();
	while (System::Update()) {
		const auto transformer = ScalableWindow::CreateTransformer();

		if (KeyF5.down()) {
			scene.get()->Reload();
		}
		if (KeyPrintScreen.down()) {
			ScreenCapture::SaveCurrentFrame();
		}

		scene.get()->Update();

		scene.updateScene();
		scene.get()->updateFlag = false;

		scene.drawScene();
		scene.get()->gothic_s(U"{}"_fmt(Profiler::FPS())).draw(Arg::bottomRight = Vec2(WindowSetting::GetRect().br()));
	}
}