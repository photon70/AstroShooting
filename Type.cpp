#include "ScriptBind.h"

#include "Scene.h"

#include "GameInterface.h"

using namespace AngelScript;

void RegisterTypes() {
	int32 r = 0;
	auto engine = ScriptManager::GetEngine();

	//Œ^‚ÌéŒ¾
	r = engine->RegisterObjectType("GameData", sizeof(GameData), asOBJ_VALUE | asOBJ_APP_CLASS_CD); assert(r >= 0);

	r = engine->RegisterObjectType("CircleF", sizeof(CircleF), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_C); assert(r >= 0);

	r = engine->RegisterObjectType("GameInterface", sizeof(GameInterface), asOBJ_VALUE | asOBJ_APP_CLASS_CD); assert(r >= 0);

	r = engine->RegisterInterface("BulletBehavior"); assert(r >= 0);
	r = engine->RegisterObjectType("BulletContainer", sizeof(BulletContainer), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_C); assert(r >= 0);
	r = engine->RegisterObjectType("BulletData", sizeof(BulletData), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_C); assert(r >= 0);

	r = engine->RegisterEnum("StageType"); assert(r >= 0);
	r = engine->RegisterInterface("EnemyBehavior"); assert(r >= 0);
	r = engine->RegisterObjectType("EnemyContainer", sizeof(EnemyContainer), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_C); assert(r >= 0);
	r = engine->RegisterObjectType("EnemyData", sizeof(EnemyData), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_C); assert(r >= 0);

	//Œ^‚Ì’è‹`
	RegisterResource();
	RegisterGameData();
	RegisterCircleF();
	RegisterGameInterface();
	RegisterBullet();
	RegisterEnemy();
}