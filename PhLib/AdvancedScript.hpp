#pragma once

#include <Siv3D.hpp>//ver3.2対応

namespace ph {
	//コルーチンを再利用する最小単位
	using CoroutineUnit = std::shared_ptr<ScriptModuleData>;

	using ScriptFunctionHandle = AngelScript::asIScriptFunction*;

	using ScriptObjectType = AngelScript::asITypeInfo*;

	inline CoroutineUnit MakeCoroutineUnit() { return std::make_shared<ScriptModuleData>(); };

	void Yield();

	void RegisterYield();//yield()を登録するだけ。コルーチン使うなら要る

	//スクリプトで定義されたクラスを扱う構造体
	//基本的にGetProp()しか使わない
	struct ScriptObject {
		using Ptr = AngelScript::asIScriptObject*;
		Ptr ptr = nullptr;

		ScriptObject();

		ScriptObject(Ptr ptr, bool isAddRef = true);

		ScriptObject(const ScriptObject& obj);

		ScriptObject(ScriptObject&& obj) noexcept;

		~ScriptObject();

		ScriptObject& operator=(Ptr ptr);

		ScriptObject& operator=(const ScriptObject& obj);

		ScriptObject& operator=(ScriptObject&& obj) noexcept;

		Ptr operator->();

		const Ptr operator->()const;

		operator bool()const;

		void AddRef();

		void Release();

		//メンバ変数取得するやつ
		template<typename Ret>
		Ret& GetProp(uint32 index) {
			return *reinterpret_cast<Ret*>(ptr->GetAddressOfProperty(index));
		}

		template<typename Ret>
		const Ret& GetProp(uint32 index)const {
			return *reinterpret_cast<Ret>(ptr->GetAddressOfProperty(index));
		}
	};

	//Abort()に置き換えたやつ
	namespace detail {
		inline void LineCallback(AngelScript::asIScriptContext* ctx, unsigned long*)
		{
			uint64* stepCounter = static_cast<uint64*>(ctx->GetUserData(AngelScript::asPWORD(static_cast<uint32>(s3d::detail::ScriptUserDataIndex::StepCounter))));

			++(*stepCounter);

			if ((*stepCounter) > 1'000'000)
			{
				ctx->Abort();
			}
		}
	}

	template <class Type>
	struct AdvancedScriptFunction;

	//クラスメソッドに対応したスクリプト関数クラス
	template <class Ret, class... Args>
	struct AdvancedScriptFunction<Ret(Args...)> {
	private:

		std::shared_ptr<ScriptModuleData> m_moduleData;

		AngelScript::asIScriptFunction* m_function = nullptr;

		void* m_objectPtr = nullptr;

		template <class Type, class ... Args2>
		void setArgs(uint32 argIndex, Type&& value, Args2&& ... args) const
		{
			setArg(argIndex++, std::forward<Type>(value));

			setArgs(argIndex, std::forward<Args2>(args)...);
		}

		template <class Type>
		void setArgs(uint32 argIndex, Type&& value) const
		{
			setArg(argIndex++, std::forward<Type>(value));
		}

		void setArgs(uint32) const
		{

		}

		template <class Type>
		void setArg(uint32 argIndex, Type&& value) const
		{
			s3d::detail::SetArg<Type>(m_moduleData, argIndex, std::forward<Type>(value));
		}

		bool execute() const
		{
			int32 steps = 0;

			if (m_moduleData->withLineCues)
			{
				m_moduleData->context->SetLineCallback(asFUNCTION(detail::LineCallback), &steps, AngelScript::asCALL_CDECL);
			}

			uint64 scriptID = m_moduleData->scriptID;
			uint64 scriptStepCounter = 0;
			m_moduleData->context->SetUserData(&scriptID, static_cast<uint32>(s3d::detail::ScriptUserDataIndex::ScriptID));
			m_moduleData->context->SetUserData(&scriptStepCounter, static_cast<uint32>(s3d::detail::ScriptUserDataIndex::StepCounter));

			const int32 r = m_moduleData->context->Execute();

			if (r != AngelScript::asEXECUTION_FINISHED && r == AngelScript::asEXECUTION_EXCEPTION)
			{
				LOG_ERROR(U"[script exception] An exception '{}' occurred."_fmt(Unicode::Widen(m_moduleData->context->GetExceptionString())));
				return false;
			}
			else if (r != AngelScript::asEXECUTION_FINISHED && r == AngelScript::asEXECUTION_ABORTED)
			{
				System::Exit();
			}

			return true;
		}

		Optional<String> tryExecute() const
		{
			int32 steps = 0;

			if (m_moduleData->withLineCues)
			{
				m_moduleData->context->SetLineCallback(asFUNCTION(detail::LineCallback), &steps, AngelScript::asCALL_CDECL);
			}

			uint64 scriptID = m_moduleData->scriptID;
			uint64 scriptStepCounter = 0;
			m_moduleData->context->SetUserData(&scriptID, static_cast<uint32>(s3d::detail::ScriptUserDataIndex::ScriptID));
			m_moduleData->context->SetUserData(&scriptStepCounter, static_cast<uint32>(s3d::detail::ScriptUserDataIndex::StepCounter));

			const int32 r = m_moduleData->context->Execute();

			if (r != AngelScript::asEXECUTION_FINISHED && r == AngelScript::asEXECUTION_EXCEPTION)
			{
				return Unicode::Widen(m_moduleData->context->GetExceptionString());
			}
			else if (r != AngelScript::asEXECUTION_FINISHED && r == AngelScript::asEXECUTION_ABORTED)
			{
				System::Exit();
			}

			return none;
		}

		template <class Type>
		Type getReturn() const
		{
			return s3d::detail::GetReturnValue<Type>(m_moduleData);
		}

	public:

		static constexpr size_t nargs = sizeof...(Args);

		template <size_t i>
		struct Arg
		{
			using type = typename std::tuple_element_t<i, std::tuple<Args...>>;
		};

		AdvancedScriptFunction() = default;

		AdvancedScriptFunction(const std::shared_ptr<ScriptModuleData>& moduleData, AngelScript::asIScriptFunction* function, void* object = nullptr)
			: m_moduleData(moduleData)
			, m_function((moduleData&& moduleData->module&& moduleData->context) ? function : nullptr)
			, m_objectPtr((moduleData&& moduleData->module&& moduleData->context) ? object : nullptr){}

		explicit operator bool() const
		{
			return static_cast<bool>(m_function);
		}
		//呼び出す
		Ret operator()(Args... args) const
		{
			if (!m_function)
			{
				return Ret();
			}

			m_moduleData->context->Prepare(m_function);
			if(m_objectPtr != nullptr)
				m_moduleData->context->SetObject(m_objectPtr);

			setArgs(0, std::forward<Args>(args)...);

			if (!execute())
			{
				return Ret();
			}

			return getReturn<Ret>();
		}
		//試しに呼び出す
		Ret tryCall(Args... args, String& exception) const
		{
			if (!m_function)
			{
				return Ret();
			}

			m_moduleData->context->Prepare(m_function);
			if (m_objectPtr != nullptr)
				m_moduleData->context->SetObject(m_objectPtr);

			setArgs(0, std::forward<Args>(args)...);

			if (const auto ex = tryExecute())
			{
				exception = ex.value();

				return Ret();
			}
			else
			{
				exception.clear();
			}

			return getReturn<Ret>();
		}
	};


	template <class Type>
	struct CoroutineScriptFunction;

	//コルーチンに対応したスクリプト関数クラス
	template <class... Args>
	struct CoroutineScriptFunction<void(Args...)> {
	private:

		std::shared_ptr<ScriptModuleData> m_moduleData;

		AngelScript::asIScriptFunction* m_function = nullptr;

		void* m_objectPtr = nullptr;

		template <class Type, class ... Args2>
		void setArgs(uint32 argIndex, Type&& value, Args2&& ... args) const
		{
			setArg(argIndex++, std::forward<Type>(value));

			setArgs(argIndex, std::forward<Args2>(args)...);
		}

		template <class Type>
		void setArgs(uint32 argIndex, Type&& value) const
		{
			setArg(argIndex++, std::forward<Type>(value));
		}

		void setArgs(uint32) const
		{

		}

		template <class Type>
		void setArg(uint32 argIndex, Type&& value) const
		{
			using type = typename std::remove_const<Type>::type;
			s3d::detail::SetArg<type>(m_moduleData, argIndex, const_cast<type>(std::forward<Type>(value)));
		}

		bool execute() const
		{
			const int32 r = m_moduleData->context->Execute();

			if (r != AngelScript::asEXECUTION_FINISHED && r == AngelScript::asEXECUTION_EXCEPTION)
			{
				LOG_ERROR(U"[script exception] An exception '{}' occurred."_fmt(Unicode::Widen(m_moduleData->context->GetExceptionString())));
				return false;
			}
			else if (r != AngelScript::asEXECUTION_FINISHED && r == AngelScript::asEXECUTION_ABORTED)
			{
				System::Exit();
			}

			return true;
		}

		Optional<String> tryExecute() const
		{
			const int32 r = m_moduleData->context->Execute();

			if (r != AngelScript::asEXECUTION_FINISHED && r == AngelScript::asEXECUTION_EXCEPTION)
			{
				return Unicode::Widen(m_moduleData->context->GetExceptionString());
			}
			else if (r != AngelScript::asEXECUTION_FINISHED && r == AngelScript::asEXECUTION_ABORTED)
			{
				System::Exit();
			}

			return none;
		}

		void Prepare()const {
			if (!m_function)
			{
				return;
			}

			m_moduleData->context->Prepare(m_function);
			if (m_objectPtr != nullptr)
				m_moduleData->context->SetObject(m_objectPtr);

			uint64 scriptID = m_moduleData->scriptID;
			uint64 scriptStepCounter = 0;
			m_moduleData->context->SetUserData(&scriptID, static_cast<uint32>(s3d::detail::ScriptUserDataIndex::ScriptID));
			m_moduleData->context->SetUserData(&scriptStepCounter, static_cast<uint32>(s3d::detail::ScriptUserDataIndex::StepCounter));
		}

	public:

		static constexpr size_t nargs = sizeof...(Args);

		template <size_t i>
		struct Arg
		{
			using type = typename std::tuple_element_t<i, std::tuple<Args...>>;
		};

		CoroutineScriptFunction()
			:m_moduleData(), m_function(nullptr), m_objectPtr(nullptr) {}

		CoroutineScriptFunction(AngelScript::asIScriptFunction* function, void* object, CoroutineUnit&& unit):
			m_moduleData(unit),
			m_function(function),
			m_objectPtr(object)
		{
			if(m_moduleData->context == nullptr)
				m_moduleData->context = ScriptManager::GetEngine()->CreateContext();
		}

		explicit operator bool() const
		{
			return static_cast<bool>(m_moduleData) && static_cast<bool>(m_function);
		}
		//呼び出す
		void operator()(Args... args) const
		{
			if (!m_moduleData)
				return;

			switch (m_moduleData->context->GetState()){
			case AngelScript::asEXECUTION_FINISHED:
			case AngelScript::asEXECUTION_UNINITIALIZED:
				Prepare();
				break;
			}

			setArgs(0, std::forward<Args>(args)...);

			if (!execute())
			{
				return;
			}

			return;
		}
		//試しに呼び出す
		void tryCall(Args... args, String& exception) const
		{
			if (!m_moduleData)
				return;

			switch (m_moduleData->context->GetState()) {
			case AngelScript::asEXECUTION_FINISHED:
			case AngelScript::asEXECUTION_UNINITIALIZED:
				Prepare();
				break;
			}

			setArgs(0, std::forward<Args>(args)...);

			if (const auto ex = tryExecute())
			{
				exception = ex.value();

				return;
			}
			else
			{
				exception.clear();
			}

			return;
		}
		//コルーチンを再利用するためのやつ。無理して使うことはない
		CoroutineUnit Release() {
			CoroutineUnit ret;
			ret.swap(m_moduleData);
			return ret;
		}
	};

	//拡張したスクリプト管理クラス
	class AdvancedScript : public Script{
	protected:
		void* _getGlobalVar(const String& decl)const;

		AngelScript::asIScriptFunction* _getObjectMethod(const String& functionDecl, const ScriptObject& object)const;

		AngelScript::asIScriptFunction* _getObjectMethod(const String& functionDecl, const ph::ScriptObjectType objectType)const;

		AngelScript::asITypeInfo* _getTypeInfo(const String& typeName)const;

	public:
		using Script::Script;
		//asIScriptModuleに直接アクセスできるだけ
		AngelScript::asIScriptModule* operator->() {
			return _getModuleData()->module;
		}
		//asIScriptModuleに直接アクセスできるだけ
		AngelScript::asIScriptModule* const operator->()const {
			return _getModuleData()->module;
		}
		//グローバル変数取り出す
		template<typename Ret>
		Ret getGlobalVar(const String& decl)const {
			if (isEmpty())
				return Ret();

			void* ptr = _getGlobalVar(decl);
			if (ptr == nullptr)
				return Ret();
			return reinterpret_cast<Ret>(*ptr);
		}
		//グローバル変数取り出す　スクリプトクラス用
		template<>
		ScriptObject getGlobalVar(const String& decl)const {;
			if (isEmpty())
				return ScriptObject();
			return ScriptObject(static_cast<AngelScript::asIScriptObject*>(_getGlobalVar(decl)));
		}

		ph::ScriptFunctionHandle getFunctionHandle(const String& decl)const {
			return _getFunction(decl);
		}
		ph::ScriptObjectType getObjectType(const String& typeName)const {
			return _getTypeInfo(typeName);
		}
		ph::ScriptFunctionHandle getObjectMethodHandle(const String& functionDecl, const String& typeName)const {
			return _getObjectMethod(functionDecl, _getTypeInfo(typeName));
		}
		ph::ScriptFunctionHandle getObjectMethodHandle(const String& functionDecl, ph::ScriptObjectType object)const {
			return _getObjectMethod(functionDecl, object);
		}

		//グローバル関数取り出す
		template <class Fty>
		[[nodiscard]] auto getFunction(const String& decl) const
		{
			const auto func = isEmpty() ? nullptr : _getFunction(decl);

			return AdvancedScriptFunction<Fty>(func ? _getModuleData() : nullptr, func);
		}
		//グローバル関数取り出す ScriptFunctionHandle使用
		template <class Fty>
		[[nodiscard]] auto getFunction(const ScriptFunctionHandle handle) const
		{
			const auto func = isEmpty() ? nullptr : handle;

			return AdvancedScriptFunction<Fty>(func ? _getModuleData() : nullptr, func);
		}

		//オブジェクトメソッド取り出す ScriptObjectでオブジェクトを指定
		template <class Fty>
		[[nodiscard]] auto getObjectMethod(const String& functionDecl, const ScriptObject& object) const
		{
			const auto func = isEmpty() ? nullptr : _getObjectMethod(functionDecl, object);

			return AdvancedScriptFunction<Fty>(func ? _getModuleData() : nullptr, func, object.ptr);
		}
		//オブジェクトメソッド取り出す 文字列でオブジェクトを指定
		template <class Fty>
		[[nodiscard]] auto getObjectMethod(const String& functionDecl, const String& objectDecl) const
		{
			const ScriptObject object = isEmpty() ? ScriptObject() : getGlobalVar<ScriptObject>(objectDecl);

			const auto func = isEmpty() ? nullptr : _getObjectMethod(functionDecl, object);
			
			return AdvancedScriptFunction<Fty>(func ? _getModuleData() : nullptr, func, object.ptr);
		}
		//オブジェクトメソッド取り出す ScriptObjectでオブジェクトを指定 ScriptFunctionHandle使用
		template <class Fty>
		[[nodiscard]] auto getObjectMethod(const ScriptFunctionHandle func, const ScriptObject& object) const
		{
			return AdvancedScriptFunction<Fty>(func ? _getModuleData() : nullptr, func, object.ptr);
		}
		//オブジェクトメソッド取り出す 文字列でオブジェクトを指定 ScriptFunctionHandle使用
		template <class Fty>
		[[nodiscard]] auto getObjectMethod(const ScriptFunctionHandle func, const String& objectDecl) const
		{
			const ScriptObject object = isEmpty() ? ScriptObject() : getGlobalVar<ScriptObject>(objectDecl);

			return AdvancedScriptFunction<Fty>(func ? _getModuleData() : nullptr, func, object.ptr);
		}

		//グローバル関数取り出す　コルーチン対応　void関数限定
		template <class Fty>
		[[nodiscard]] auto getCoroutineFunction(const String& decl, CoroutineUnit&& coroutine = MakeCoroutineUnit()) const
		{
			const auto func = isEmpty() ? nullptr : _getFunction(decl);

			return CoroutineScriptFunction<Fty>(func, nullptr, std::move(coroutine));
		}
		//グローバル関数取り出す　コルーチン対応　void関数限定 ScriptFunctionHandle使用
		template <class Fty>
		[[nodiscard]] auto getCoroutineFunction(const ScriptFunctionHandle handle, CoroutineUnit&& coroutine = MakeCoroutineUnit()) const
		{
			const auto func = isEmpty() ? nullptr : handle;

			return CoroutineScriptFunction<Fty>(func, nullptr, std::move(coroutine));
		}

		//オブジェクトメソッド取り出す ScriptObjectでオブジェクトを指定　コルーチン対応　void関数限定
		template <class Fty>
		[[nodiscard]] auto getCoroutineObjectMethod(const String& functionDecl, const ScriptObject& object, CoroutineUnit&& coroutine = MakeCoroutineUnit()) const
		{
			const auto func = isEmpty() ? nullptr : _getObjectMethod(functionDecl, object);

			return CoroutineScriptFunction<Fty>(func, object.ptr, std::move(coroutine));
		}
		//オブジェクトメソッド取り出す 文字列でオブジェクトを指定　コルーチン対応　void関数限定 ScriptFunctionHandle使用
		template <class Fty>
		[[nodiscard]] auto getCoroutineObjectMethod(const String& functionDecl, const String& objectDecl, CoroutineUnit&& coroutine = MakeCoroutineUnit()) const
		{
			const ScriptObject object = isEmpty() ? ScriptObject() : getGlobalVar<ScriptObject>(objectDecl);

			const auto func = isEmpty() ? nullptr : _getObjectMethod(functionDecl, object);

			return CoroutineScriptFunction<Fty>(func, object.ptr, std::move(coroutine));
		}
		//オブジェクトメソッド取り出す ScriptObjectでオブジェクトを指定　コルーチン対応　void関数限定 ScriptFunctionHandle使用
		template <class Fty>
		[[nodiscard]] auto getCoroutineObjectMethod(const ScriptFunctionHandle func, const ScriptObject& object, CoroutineUnit&& coroutine = MakeCoroutineUnit()) const
		{
			return CoroutineScriptFunction<Fty>(func, object.ptr, std::move(coroutine));
		}
		//オブジェクトメソッド取り出す 文字列でオブジェクトを指定　コルーチン対応　void関数限定 ScriptFunctionHandle使用
		template <class Fty>
		[[nodiscard]] auto getCoroutineObjectMethod(const ScriptFunctionHandle func, const String& objectDecl, CoroutineUnit&& coroutine = MakeCoroutineUnit()) const
		{
			const ScriptObject object = isEmpty() ? ScriptObject() : getGlobalVar<ScriptObject>(objectDecl);

			return CoroutineScriptFunction<Fty>(func, object.ptr, std::move(coroutine));
		}

		//スクリプトクラスのコンストラクタを取得
		template <class Fty = ScriptObject()>
		[[nodiscard]] auto getObjectFactory(const String& typeName, const String& factoryDecl = U"") {
			const auto type = isEmpty() ? nullptr : _getTypeInfo(typeName);

			const auto func = isEmpty() || type == nullptr ? nullptr : type->GetFactoryByDecl(factoryDecl == U"" ? (typeName + U"@ " + typeName + U"()").narrow().c_str() : factoryDecl.narrow().c_str());
			
			return AdvancedScriptFunction<Fty>(func ? _getModuleData() : nullptr, func);
		}
	};
}

//関数の引数にScriptObjectを対応させる
template <>
inline void s3d::detail::SetArg<ph::ScriptObject>(const std::shared_ptr<ScriptModuleData>& moduleData, uint32 argIndex, const ph::ScriptObject& value)
{
	moduleData->context->SetArgAddress(argIndex, static_cast<void*>(value.ptr));
}
//関数の戻り値にScriptObjectを対応させる
template <>
inline ph::ScriptObject s3d::detail::GetReturnValue(const std::shared_ptr<ScriptModuleData>& moduleData)
{
	return *static_cast<AngelScript::asIScriptObject**>(moduleData->context->GetAddressOfReturnValue());
}