#include "Scene.h"

GameData::GameData() {
//	TextReader txt(GetResource(U"AS/string.txt");
	String str[9 * 5] = {
U"あいうえお はひふへほ",
U"かきくけこ まみむめも",
U"さしすせそ らりるれろ",
U"たちつてと やゆよーゝ",
U"なにぬねの わをん゛゜",
U"あいうえお ばびぶべぼ",
U"がぎぐげご まみむめも",
U"ざじずぜぞ らりるれろ",
U"だぢづでど やゆよーゞ",
U"なにぬねの わをん゛゜",
U"あいうえお ぱぴぷぺぽ",
U"かきくけこ まみむめも",
U"さしすせそ らりるれろ",
U"たちつてと やゆよーゝ",
U"なにぬねの わをん゛゜",
U"ぁぃぅぇぉ はひふへほ",
U"かきくけこ まみむめも",
U"さしすせそ らりるれろ",
U"たちってと ゃゅょーゝ",
U"なにぬねの ゎをん゛゜",
U"アイウエオ ハヒフヘホ",
U"カキクケコ マミムメモ",
U"サシスセソ ラリルレロ",
U"タチツテト ヤユヨーヽ",
U"ナニヌネノ ワヲン゛゜",
U"アイヴエオ バビブベボ",
U"ガギグゲゴ マミムメモ",
U"ザジズゼゾ ラリルレロ",
U"ダヂヅデド ヤユヨーヾ",
U"ナニヌネノ ワヲン゛゜",
U"アイウエオ パピプペポ",
U"カキクケコ マミムメモ",
U"サシスセソ ラリルレロ",
U"タチツテト ヤユヨーヽ",
U"ナニヌネノ ワヲン゛゜",
U"ァィゥェォ ハヒフヘホ",
U"カキクケコ マミムメモ",
U"サシスセソ ラリルレロ",
U"タチツテト ャュョーヽ",
U"ナニヌネノ ワヲン゛゜",
U"abcdefghijk",
U"lmnopqrstuv",
U"wxyzABCDEFG",
U"HIJKLMNOPQR",
U"STUVWXYZ!?_" };
	int n = 0;
	for (auto& i : nameString) {
		for (auto& k : i) {
		//	txt.readLine(str);
			for (auto m : step(11)) {
				k[m] = str[n][m];
			}
			++n;
		}
	}

	printerFlag = FileSystem::Exists(U"PrintApp.exe");
}

//void GameData::RegisterData() {
//	script.reload();
//
//	s3d::ClearPrint();
//	for (auto& str : script.getMessages()) {
//		s3d::Print.operator<<(str);
//	}
//
//	Init();
//}

void GameData::Reload() {
	bool flag = true;

#ifdef ReadPrecompile
	ph::BinaryStreamR wr(GetResource(U"AS/source.dat"));
	script->LoadByteCode(&wr);
#endif // ReadPrecompile

#ifndef ReadPrecompile
	while (flag) {
		script.reload();

		s3d::ClearPrint();
		for (auto& str : script.getMessages()) {
			s3d::Print.operator<<(str);
		}

		if (!script.compiled() && System::Update()) {
			auto selec = System::ShowMessageBox(U"スクリプトのエラーです。\nコードを更新したらOKを押してください", s3d::MessageBoxButtons::OKCancel);
			if (selec == s3d::MessageBoxSelection::Cancel) {
				s3d::System::Exit();
			}
		}
		else {
			flag = false;
		}
	}
#endif // !ReadPrecompile


#ifdef WritePrecompile
	ph::BinaryStreamW r(GetResource(U"AS/source.dat"));
	script->SaveByteCode(&r);
#endif //WriteProcompile
	
	Init();

	updateFlag = true;
}

void GameData::Init() {
	//コンテナ消去
	enemy.clear();
	pageStr.clear();
	pageBrief.clear();
	manualStr.clear();
	pageLock.clear();

	script.getFunction<void(GameData&)>(U"RegisterData")(*this);

	save = SaveData(U"AS/score.dat", enemy.size());
	ApplySetting();

	//コンテナをリロード

}

void  GameData::Update() {
	auto gamepad = Gamepad(0);

	right.update(gamepad);
	left.update(gamepad);
	up.update(gamepad);
	down.update(gamepad);
}


void GameData::ApplySetting() {

	if (auto gamepad = Gamepad(0)) {
		ok = default_ok | gamepad.buttons[save.gamepadNum[0]];
		cancel = default_cancel | gamepad.buttons[save.gamepadNum[1]];
		shift = default_shift | gamepad.buttons[save.gamepadNum[2]];
		escape = default_escape | gamepad.buttons[save.gamepadNum[3]];
	}

	switch (save.windowSize) {
	default:
	case 0:
		WindowSetting::size = Size(1280, 960);
		break;
	case 1:
		WindowSetting::size = Size(960, 720);
		break;
	case 2:
		WindowSetting::size = Size(640, 480);
		break;
	}
	Window::Resize(WindowSetting::GetSize());
}

void GameData::Play(Audio& audio) {
	audio.setVolume((float)save.soundVolume / 100.f);
	audio.play();
}

void GameData::PlayBGM(Audio& audio) {
	audio.setVolume((float)save.bgmVolume / 100.f);
	audio.play();
}

void GameData::PlayOneShot(Audio& audio) {
	audio.playOneShot((float)save.soundVolume / 100.f);
}

using namespace AngelScript;

using TypeName = GameData;

static void Construct(const TypeName& obj, TypeName* self) {
	new(self) TypeName(obj);
}

static void Destruct(TypeName* self) {
	self->~GameData();
}

void RegisterGameData() {
	int32 r = 0;
	auto engine = ScriptManager::GetEngine();

	constexpr char typeName[] = "GameData";

	//コンテナ登録関数
	r = engine->RegisterObjectMethod(typeName, "void RegisterEnemy(const EnemyContainer &in)", asMETHODPR(TypeName, RegisterEnemy, (const EnemyContainer&), void), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(typeName, "void RegisterPage(const String &in, const String &in, int)", asMETHODPR(TypeName, RegisterPage, (const String&, const String&, int), void), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod(typeName, "void RegisterManual(const String &in)", asMETHODPR(TypeName, RegisterManual, (const String&), void), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f(const GameData &in)", asFUNCTION(Construct), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Destruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
}