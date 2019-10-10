#include "AdvancedScript.hpp"

void ph::Yield() {
	AngelScript::asIScriptContext* ctx = AngelScript::asGetActiveContext();
	if (ctx) {
		//コンテキストを中断して次のコルーチンを再開できるようにする
		ctx->Suspend();
	}
}

void ph::RegisterYield() {
	auto engine = ScriptManager::GetEngine();
	int32 r = 0;

	r = engine->RegisterGlobalFunction("void yield()", AngelScript::asFUNCTIONPR(Yield, (), void), AngelScript::asCALL_CDECL); assert(r >= 0);
}

ph::ScriptObject::ScriptObject()
	:ptr(nullptr) {}

ph::ScriptObject::ScriptObject(Ptr ptr, bool isAddRef)
	:ptr(ptr) {
	if (isAddRef)
		AddRef();
}

ph::ScriptObject::ScriptObject(const ScriptObject& obj) {
	this->ptr = obj.ptr;
	AddRef();
}

ph::ScriptObject::ScriptObject(ScriptObject&& obj) noexcept{
	this->ptr = obj.ptr;
	obj.ptr = nullptr;
}

ph::ScriptObject::~ScriptObject() {
	Release();
}

ph::ScriptObject& ph::ScriptObject::operator=(Ptr obj) {
	Release();
	this->ptr = obj;
	AddRef();
	return *this;
}

ph::ScriptObject& ph::ScriptObject::operator=(const ScriptObject& obj) {
	Release();
	this->ptr = obj.ptr;
	AddRef();
	return *this;
}

ph::ScriptObject& ph::ScriptObject::operator=(ScriptObject&& obj) noexcept {
	Release();
	this->ptr = obj.ptr;
	obj.ptr = nullptr;
	return *this;
}

ph::ScriptObject::Ptr ph::ScriptObject::operator->() {
	return ptr;
}

const ph::ScriptObject::Ptr ph::ScriptObject::operator->()const {
	return ptr;
}

ph::ScriptObject::operator bool()const {
	return ptr != nullptr;
}

void ph::ScriptObject::AddRef() {
	if (ptr != nullptr)
		ptr->AddRef();
}

void ph::ScriptObject::Release() {
	if (ptr != nullptr)
		ptr->Release();
}

void* ph::AdvancedScript::_getGlobalVar(const String& decl)const {
	auto m = _getModuleData()->module;

	const bool byDecl = decl.includes(L' ');

	int32 index = byDecl ? m->GetGlobalVarIndexByDecl(decl.narrow().c_str())
		: m->GetGlobalVarIndexByName(decl.narrow().c_str());

	if (index < 0)
		return nullptr;

	return m->GetAddressOfGlobalVar(index);
}

AngelScript::asIScriptFunction* ph::AdvancedScript::_getObjectMethod(const String& functionDecl, const ScriptObject& object)const {
	auto type = object ? object->GetObjectType() : nullptr;

	if (type == nullptr)
		return nullptr;

	const bool byDecl = functionDecl.includes(L' ');

	auto ptr = byDecl ? type->GetMethodByDecl(functionDecl.narrow().c_str())
		: type->GetMethodByName(functionDecl.narrow().c_str());

	return ptr;

	/*auto type = object ? object->GetObjectType() : nullptr;

	if (type == nullptr)
		return nullptr;


	auto it = methods.find(functionDecl);

	if (it == methods.end())
	{
		const bool byDecl = functionDecl.includes(L' ');

		AngelScript::asIScriptFunction* func =
			byDecl ? type->GetMethodByDecl(functionDecl.narrow().c_str())
			: type->GetMethodByName(functionDecl.narrow().c_str());

		if (func == 0) {
			return nullptr;
		}

		it = methods.emplace(functionDecl, func).first;
	}

	return it->second;*/
}

AngelScript::asIScriptFunction* ph::AdvancedScript::_getObjectMethod(const String& functionDecl, const ph::ScriptObjectType type)const {
	if (type == nullptr)
		return nullptr;

	const bool byDecl = functionDecl.includes(L' ');

	auto ptr = byDecl ? type->GetMethodByDecl(functionDecl.narrow().c_str())
		: type->GetMethodByName(functionDecl.narrow().c_str());

	return ptr;
}

AngelScript::asITypeInfo* ph::AdvancedScript::_getTypeInfo(const String& typeName)const {
	auto m = _getModuleData()->module;
	
	return m->GetTypeInfoByName(typeName.narrow().c_str());
}