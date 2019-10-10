#pragma once

#include <Siv3D.hpp>

#include "Header.h"

#include "GameInterface.h"

//敵の基本情報
struct EnemyContainer {
	String name;
	String nickname;
	String behaviorName;
	String txt;
	int timeLimit;
	int targetNumber;
};

//敵の持つ可変データ
struct EnemyData {
	Float2 pos = Float2(0, -200);

	EnemyData() = default;

	EnemyData(const Float2& pPos) {
		pos = pPos;
	}

	EnemyData SetPos(const Float2& pPos) {
		pos = pPos;
		return *this;
	}

	EnemyData Set(const Float2& pPos) {
		pos = pPos;
		return *this;
	}
};

struct Enemy {
public:
	using UpdateFunc = void(GameInterface&, EnemyData&);

	static ph::ScriptFunctionHandle update, draw;
	const EnemyContainer& container;
	EnemyData data;
	ph::ScriptObject behavior;
	ph::CoroutineScriptFunction<UpdateFunc> coUpdate;

	Enemy(const EnemyContainer& container, ph::ScriptObject behavior) :
		container(container), behavior(behavior) {
	}

	void Update(ph::AdvancedScript& script, GameInterface& inter) {
		if (!coUpdate)
			coUpdate = script.getCoroutineObjectMethod<UpdateFunc>(update, behavior);
		coUpdate(inter, data);
	}

	void Draw(ph::AdvancedScript& script)const {
		script.getObjectMethod<void(const EnemyData&)>(draw, behavior)(data);
	}
};