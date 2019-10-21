#include "Bullet.h"

ph::ScriptFunctionHandle Bullet::update, Bullet::draw;

using namespace AngelScript;

using ContainerType = BulletContainer;
using DataType = BulletData;

static void ContainerConstruct(ContainerType* self)
{
	new(self) ContainerType();
}

static void DataConstruct(DataType* self)
{
	new(self) DataType();
}

static void DataConstructFF(const Float2& pPos, float pAngle, float pRad, int pCount, DataType* self)
{
	new(self) DataType(pPos, pAngle, pRad, pCount);
}

void RegisterBullet() {
	int32 r = 0;
	auto engine = ScriptManager::GetEngine();


	{
		constexpr char typeName[] = "BulletBehavior";

		r = engine->RegisterInterfaceMethod(typeName, "void Update(GameInterface&, BulletData&)"); assert(r >= 0);
		r = engine->RegisterInterfaceMethod(typeName, "void Draw(const BulletData&, const BulletContainer&)"); assert(r >= 0);

		auto type = engine->GetTypeInfoByName(typeName);
		Bullet::update = type->GetMethodByName("Update");
		Bullet::draw= type->GetMethodByName("Draw");
	}

	{
		constexpr char typeName[] = "BulletContainer";

		r = engine->RegisterObjectProperty(typeName, "Texture texture", asOFFSET(ContainerType, texture)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "ColorF color", asOFFSET(ContainerType, color)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "float scale", asOFFSET(ContainerType, scale)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "float rad", asOFFSET(ContainerType, rad)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "int score", asOFFSET(ContainerType, score)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "bool drawFlag", asOFFSET(ContainerType, drawFlag)); assert(r >= 0);
		
		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ContainerConstruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
	}

	{
		constexpr char typeName[] = "BulletData";

		r = engine->RegisterObjectProperty(typeName, "Float2 pos", asOFFSET(DataType, pos)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "Float2 angle", asOFFSET(DataType, angle)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "float radian", asOFFSET(DataType, radian)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "int count", asOFFSET(DataType, count)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "bool autoDelete", asOFFSET(DataType, autoDelete)); assert(r >= 0);
		r = engine->RegisterObjectProperty(typeName, "bool deleteFlag", asOFFSET(DataType, deleteFlag)); assert(r >= 0);
	
		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DataConstruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_CONSTRUCT, "void f(const Float2&, float, float pRad = 1.f, int pCount = 0)", asFUNCTION(DataConstructFF), asCALL_CDECL_OBJLAST); assert(r >= 0);

		r = engine->RegisterObjectMethod(typeName, "BulletData SetPos(const Float2& in)", asMETHODPR(DataType, SetPos, (const Float2&), BulletData), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(typeName, "BulletData AddPos(const Float2& in)", asMETHODPR(DataType, AddPos, (const Float2&), BulletData), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(typeName, "BulletData SetAngle(float, float pRad = 1.f)", asMETHODPR(DataType, SetAngle, (float, float), BulletData), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(typeName, "BulletData AddAngle(float, float pRad = 0)", asMETHODPR(DataType, AddAngle, (float, float), BulletData), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(typeName, "BulletData SetCount(int)", asMETHODPR(DataType, SetCount, (int), BulletData), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(typeName, "BulletData AutoDelete(bool flag = false)", asMETHODPR(DataType, AutoDelete, (bool), BulletData), asCALL_THISCALL); assert(r >= 0);
		r = engine->RegisterObjectMethod(typeName, "BulletData Set(const Float2& in, float, float pRad = 1.f, int pCount = 0)", asMETHODPR(DataType, Set, (const Float2&, float, float, int), BulletData), asCALL_THISCALL); assert(r >= 0);
	}
}