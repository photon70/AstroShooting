#include "Scene.h"

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
	ph::BinaryStreamR wr(U"AS/source.dat");
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
	ph::BinaryStreamW r(U"AS/source.dat");
	script->SaveByteCode(&r);
#endif //WriteProcompile
	
	Init();

	updateFlag = true;
}

void GameData::Init() {
	//コンテナ消去
	enemy.clear();
	pageStr.clear();

	script.getFunction<void(GameData&)>(U"RegisterData")(*this);

	save = SaveData(U"AS/score.dat", enemy.size());
	
	//コンテナをリロード

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
	r = engine->RegisterObjectMethod(typeName, "void RegisterPage(const String &in)", asMETHODPR(TypeName, RegisterPage, (const String&), void), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f(const GameData &in)", asFUNCTION(Construct), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Destruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
}