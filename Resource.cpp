#include "Resource.h"

void RegisterResource() {
	auto engine = ScriptManager::GetEngine();
	int32 r = 0;

	r = engine->RegisterGlobalFunction("String GetResource(String)", AngelScript::asFUNCTIONPR(GetResource, (String), String), AngelScript::asCALL_CDECL); assert(r >= 0);
}