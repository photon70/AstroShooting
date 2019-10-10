#pragma once

#include "Header.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "GameInterface.h"
#include "Save.h"

struct GameData {
#ifdef ReadPrecompile
	ph::AdvancedScript script = ph::AdvancedScript(U"AS/dammy.as");
#endif // ReadPrecompile
#ifndef ReadPrecompile
	ph::AdvancedScript script = ph::AdvancedScript(U"script/main.as");
#endif // !ReadPrecompile

	Rect field = Rect(Arg::center(0, 0), 720, 900);

	Font mk_title = Font(80, U"AS/851MkPOP_002.ttf");
	Font mk_menu = Font(40, U"AS/851MkPOP_002.ttf");
	Font mk_menu_stageTitle = Font(36, U"AS/851MkPOP_002.ttf");
	Font mk_menu_s = Font(28, U"AS/851MkPOP_002.ttf");
	Font mk_menu_l = Font(64, U"AS/851MkPOP_002.ttf");
	Font mk_menu_txt = Font(45, U"AS/851MkPOP_002.ttf");
	Font mk_pause_title = Font(48, U"AS/851MkPOP_002.ttf", FontStyle::Bold);
	Font gothic_pause = Font(36);
	Font gothic_s = Font(20, Typeface::Heavy);
	Font gothic_l = Font(64);

	ColorF backColor = ColorF(0.3, 0.4, 0.5);

	KeyGroup right = KeyGroup(KeyRight);
	KeyGroup left = KeyGroup(KeyLeft);
	KeyGroup up = KeyGroup(KeyUp);
	KeyGroup down = KeyGroup(KeyDown);
	KeyGroup ok = KeyGroup(KeyEnter, KeyZ);
	KeyGroup cancel = KeyGroup(KeyBackspace, KeyX);
	KeyGroup shift = KeyGroup(KeyShift);
	KeyGroup escape = KeyGroup(KeyEscape);

	SaveData save;

	//ステージ選択情報
	int page = 0;
	int select = 0;
	int maxPage;

	bool isPageForward = true;

	//コンテナ定義
	Array<EnemyContainer> enemy;
	Array<String> pageStr;

	bool updateFlag = false;

	//void RegisterData();
	void Reload();
	void Init();

	//選択されているステージ番号　存在しないときは-1
	int GetStageNum()const {
		auto n = page * 6 + select;
		if (n >= enemy.size())return - 1;
		return n;
	}

	void GoNextStage() {
		if (GetStageNum() < enemy.size()) {
			if (select == 5) {
				++page;
				select = 0;
			}
			else {
				++select;
			}
		}
	}

	//スクリプト用のコンテナ登録関数
	void RegisterEnemy(const EnemyContainer& obj) {
		enemy << obj;
	}
	void RegisterPage(const String& obj) {
		pageStr << obj;
	}
};

using SceneMaster = SceneManager<String, GameData>;