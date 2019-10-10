#include "GameInterface.h"

#include "Bullet.h"

void GameInterface::RegisterBullet(const BulletContainer& container, BulletData data, AngelScript::asIScriptObject* behavior) {
	bulletBuf.push_back(std::make_unique<Bullet>(container, data, ph::ScriptObject(behavior, false)));
}


using namespace AngelScript;

using TypeName = GameInterface;

static void Construct(TypeName* self) {
	new(self) TypeName(Float2::Zero(), Float2::Zero());
}

static void Destruct(TypeName* self) {
	self->~GameInterface();
}

void RegisterGameInterface() {
	int32 r = 0;
	auto engine = ScriptManager::GetEngine();

	constexpr char typeName[] = "GameInterface";

	r = engine->RegisterObjectProperty(typeName, "Float2 player", asOFFSET(TypeName, player)); assert(r >= 0);
	r = engine->RegisterObjectProperty(typeName, "Float2 enemy", asOFFSET(TypeName, enemy)); assert(r >= 0);

	r = engine->RegisterObjectMethod(typeName, "void RegisterBullet(const BulletContainer&, BulletData, BulletBehavior@)", asMETHODPR(TypeName, RegisterBullet, (const BulletContainer&, BulletData, AngelScript::asIScriptObject*), void), asCALL_THISCALL); assert(r >= 0);

	r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Construct), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Destruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
}