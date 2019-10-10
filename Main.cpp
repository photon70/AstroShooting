#include "Scene.h"
#include "TitleScene.h"
#include "MenuScene.h"
#include "GameScene.h"

#include "ScriptBind.h"

void Main()
{
	Window::Resize(1280, 960);
	System::SetExitEvent(WindowEvent::CloseButton);

	ph::RegisterYield();
	RegisterTypes();

	SceneMaster scene;
	scene.add<TitleScene>(U"Title");
	scene.add<MenuScene>(U"Menu");
	scene.add<GameScene>(U"Game");
	
	Graphics::SetBackground(scene.get()->backColor);
	scene.setFadeColor(scene.get()->backColor);

	scene.get()->Reload();

	System::Update();

	while (System::Update()) {

		if (KeyF5.down()) {
			scene.get()->Reload();
		}
		if (KeyPrintScreen.down()) {
			ScreenCapture::SaveCurrentFrame();
		}

		scene.updateScene();
		scene.get()->updateFlag = false;

		scene.drawScene();
		scene.get()->gothic_s(U"{}"_fmt(Profiler::FPS())).draw(Arg::bottomRight = Vec2(Window::ClientRect().br()));
	}
}