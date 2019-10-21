#include "Enemy.h"

#include "Bullet.h"

ph::ScriptFunctionHandle Enemy::update, Enemy::draw;

using namespace AngelScript;

using ContainerType = EnemyContainer;
using DataType = EnemyData;

static void ContainerConstruct(ContainerType* self)
{
	new(self) ContainerType();
}

static void DataConstruct(DataType* self)
{
	new(self) DataType();
}

static void DataConstructF(const Float2& pPos, DataType* self)
{
	new(self) DataType(pPos);
}

void RegisterEnemy() {
	int32 r = 0;
	auto engine = ScriptManager::GetEngine();


	{
		constexpr char typeName[] = "EnemyBehavior";

		r = engine->RegisterInterfaceMethod(typeName, "void Update(GameInterface&, EnemyData&)"); assert(r >= 0);
		r = engine->RegisterInterfaceMethod(typeName, "void Draw(const EnemyData&)"); assert(r >= 0);
		//		r = engine->RegisterInterfaceMethod(typeName, "void Draw(const EnemyData&)"); assert(r >= 0);

		auto type = engine->GetTypeInfoByName(typeName);
		Enemy::update = type->GetMethodByName("Update");
		Enemy::draw = type->GetMethodByName("Draw");
	}

	{
		constexpr char typeName[] = "EnemyContainer";

	//	r = engine->RegisterObjectProperty(typeName, "Texture texture", asOFFSET(ContainerType, texture)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "int timeLimit", asOFFSET(ContainerType, timeLimit)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "int targetNumber", asOFFSET(ContainerType, targetNumber)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "String name", asOFFSET(ContainerType, name)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "String nickname", asOFFSET(ContainerType, nickname)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "String behaviorName", asOFFSET(ContainerType, behaviorName)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "String txt", asOFFSET(ContainerType, txt)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "int stageType", asOFFSET(ContainerType, stageType)); assert(r >= 0);

		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ContainerConstruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
	}

	{
		constexpr char typeName[] = "EnemyData";

		r = engine->RegisterObjectProperty(typeName, "Float2 pos", asOFFSET(DataType, pos)); assert(r >= 0);

		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DataConstruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f(const Float2&)", asFUNCTION(DataConstructF), asCALL_CDECL_OBJLAST); assert(r >= 0);

		r = engine->RegisterObjectMethod(typeName, "EnemyData SetPos(const Float2& in)", asMETHODPR(DataType, SetPos, (const Float2&), EnemyData), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(typeName, "EnemyData Set(const Float2& in)", asMETHODPR(DataType, SetPos, (const Float2&), EnemyData), asCALL_THISCALL); assert(r >= 0);
	}

	{
		constexpr char typeName[] = "StageType";

		r = engine->RegisterEnumValue(typeName, "Normal", (int)Enemy::Normal); assert(r >= 0);
		r = engine->RegisterEnumValue(typeName, "SuddenDeath", (int)Enemy::SuddenDeath); assert(r >= 0);
		r = engine->RegisterEnumValue(typeName, "Tutorial", (int)Enemy::Tutorial); assert(r >= 0);

	}
}