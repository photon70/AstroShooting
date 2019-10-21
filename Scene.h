#pragma once

#include "Header.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "GameInterface.h"
#include "Save.h"
#include "Resource.h"
#include "ArrowKey.h"

struct GameData {
#ifdef ReadPrecompile
	ph::AdvancedScript script = ph::AdvancedScript(GetResource(U"AS/dammy.as"));
#endif // ReadPrecompile
#ifndef ReadPrecompile
	ph::AdvancedScript script = ph::AdvancedScript(U"script/main.as");
#endif // !ReadPrecompile

	Rect field = Rect(Arg::center(0, 0), 720, 900);

	Font mk_title = Font(80, GetResource(U"AS/851MkPOP_002.ttf"));
	Font mk_menu = Font(40, GetResource(U"AS/851MkPOP_002.ttf"));
	Font mk_menu_stageTitle = Font(36, GetResource(U"AS/851MkPOP_002.ttf"));
	Font mk_menu_s = Font(28, GetResource(U"AS/851MkPOP_002.ttf"));
	Font mk_menu_l = Font(64, GetResource(U"AS/851MkPOP_002.ttf"));
	Font mk_menu_txt = Font(45, GetResource(U"AS/851MkPOP_002.ttf"));
	Font mk_pause_title = Font(48, GetResource(U"AS/851MkPOP_002.ttf"), FontStyle::Bold);
	Font gothic_pause = Font(36);
	Font gothic_s = Font(20, Typeface::Heavy);
	Font gothic_manual = Font(32);
	Font gothic_m = Font(48);
	Font gothic_l = Font(64);

	Audio aOk = Audio(GetResource(U"AS/ok.mp3"));
	Audio aCancel = Audio(GetResource(U"AS/cancel.mp3"));
	Audio aSelect = Audio(GetResource(U"AS/select.mp3"));
	Audio aPage = Audio(GetResource(U"AS/page.mp3"));
	Audio aShutter = Audio(GetResource(U"AS/shutter.mp3"));
	Audio aMove = Audio(GetResource(U"AS/move.mp3"));
	Audio aFlash = Audio(GetResource(U"AS/flash.mp3"));
	Audio aHit = Audio(GetResource(U"AS/hit.mp3"));
	Audio aCharge = Audio(GetResource(U"AS/charge.mp3"));
	Audio aCharged = Audio(GetResource(U"AS/charged.mp3"));
	Audio aChange = Audio(GetResource(U"AS/change.mp3"));
	
	Audio aStar1 = Audio(GetResource(U"AS/star1.mp3"));
	Audio aStar2 = Audio(GetResource(U"AS/star2.mp3"));
	Audio aStardust = Audio(GetResource(U"AS/stardust.mp3"), Arg::loop = true);

	Audio aClear = Audio(GetResource(U"AS/clear.mp3"));
	Audio aOver = Audio(GetResource(U"AS/over.mp3"));

	Audio aTitle = Audio(GetResource(U"AS/title.mp3"), Arg::loop = true);
	Array<Audio> aBGM = {
		Audio(GetResource(U"AS/page1.mp3"), Arg::loop = true),
		Audio(GetResource(U"AS/page2.mp3"), Arg::loop = true),
		Audio(GetResource(U"AS/page3.mp3"), Arg::loop = true),
		Audio(GetResource(U"AS/page4.mp3"), Arg::loop = true),
		Audio(GetResource(U"AS/page5.mp3"), Arg::loop = true)
	};

	ColorF backColor = ColorF(0.3, 0.4, 0.5);

	const KeyGroup default_ok = KeyGroup(KeyEnter, KeyZ);
	const KeyGroup default_cancel = KeyGroup(KeyBackspace, KeyX);
	const KeyGroup default_shift = KeyGroup(KeyShift);
	const KeyGroup default_escape = KeyGroup(KeyEscape);

	ArrowKey  right = ArrowKey(ArrowKey::Right);
	ArrowKey  left = ArrowKey(ArrowKey::Left);
	ArrowKey  up = ArrowKey(ArrowKey::Up);
	ArrowKey  down = ArrowKey(ArrowKey::Down);
	KeyGroup  ok = default_ok;
	KeyGroup  cancel = default_cancel;
	KeyGroup  shift = default_shift;
	KeyGroup  escape = default_escape;

	char32 nameString[9][5][11];

	SaveData save;
	String playerName;

	//ステージ選択情報
	int page = 0;
	bool pageFlag = true;
	int select = 0;
	int maxPage;

	int manualPage = 0;
	int manualSelect = 0;

	int availablePage = 0;

	bool isPageForward = true;

	bool printerFlag = true;

	//コンテナ定義
	Array<EnemyContainer> enemy;
	Array<String> pageStr;
	Array<String> pageBrief;
	Array<String> manualStr;
	Array<int> pageLock;

	bool updateFlag = false;

	GameData();

	//void RegisterData();
	void Reload();
	void Init();
	void Update();

	void ApplySetting();
	void Play(Audio& audio);
	void PlayBGM(Audio& audio);
	void PlayOneShot(Audio& audio);

	//選択されているステージ番号　存在しないときは-1
	int GetStageNum()const {
		auto n = page * 6 + select;
		if (n >= enemy.size())return - 1;
		return n;
	}

	void GoNextStage() {
		if (StageAvailable(GetStageNum())) {
			if (select == 5) {
				++page;
				select = 0;
			}
			else {
				++select;
			}
		}
	}

	bool StageAvailable(int n) {
		if (n < 0 || n >= enemy.size())
			return false;
		if (n < availablePage * 6)
			return true;
		else return false;
	}

	bool CheckPageAvailable() {
		int nowClear = 0;
		for (auto& s : save.data) {
			if (s.hiScore > 0)
				++nowClear;
		}
		int buf = 0;
		for (int i = 0; i < pageLock.size(); ++i) {
			if (nowClear >= pageLock[i])
				buf = i;
		}
		bool ret = (availablePage < buf);
		availablePage = buf;
		return ret;
	}

	//スクリプト用のコンテナ登録関数
	void RegisterEnemy(const EnemyContainer& obj) {
		enemy << obj;
	}
	void RegisterPage(const String& str, const String& brief, int lock) {
		pageStr << str;
		pageBrief << brief;
		pageLock << lock;
	}
	void RegisterManual(const String& str) {
		manualStr << str;
	}
};

using SceneMaster = SceneManager<String, GameData>;