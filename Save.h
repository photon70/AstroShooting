#pragma once

#include "Header.h"

struct SaveUnit{
	int hiScore = 0;
	int total = 0;
	int score = 0;
	String cameraman = U"";
	int medal = 0;
};

struct SaveData {
	Array<SaveUnit> data;
	String file;

	int gamepadNum[4] = {};
	int bgmVolume = 100;
	int soundVolume = 100;
	int windowSize = 0;
	bool hardMode = false;
	bool shotMode = false;

	SaveData() = default;

	SaveData(String file, int num):
		data(num, SaveUnit()),
		file(file){
		Load();
	}

	SaveUnit& operator [](int index) {
		if (data.size() <= index) {
			for (auto i : step(index - data.size() + 1))data << SaveUnit();
		}
		return data[index];
	}

	void Load() {
		TextReader txt(file);

		String str;
		if (txt.readLine(str))gamepadNum[0] = Parse<int>(str);
		if (txt.readLine(str))gamepadNum[1] = Parse<int>(str);
		if (txt.readLine(str))gamepadNum[2] = Parse<int>(str);
		if (txt.readLine(str))gamepadNum[3] = Parse<int>(str);
		if (txt.readLine(str))bgmVolume = Parse<int>(str);
		if (txt.readLine(str))soundVolume = Parse<int>(str);
		if (txt.readLine(str))windowSize = Parse<int>(str);
		if (txt.readLine(str))hardMode = Parse<bool>(str);
		if (txt.readLine(str))shotMode = Parse<bool>(str);

		{
			std::set<int> set;
			bool flag = false;
			for (auto i : gamepadNum) {
				if (set.contains(i)) {
					flag = true;
				}
				else {
					set.insert(i);
				}
			}
			if (flag) {
				for (auto i : step(4)) {
					gamepadNum[i] = i;
				}
			}
		}

		for (auto& d : data) {
			d = SaveUnit();
			if (txt.readLine(str))d.hiScore = Parse<int>(str);
			if (txt.readLine(str))d.total = Parse<int>(str);
			if (txt.readLine(str))d.score= Parse<int>(str);
			if (txt.readLine(str))d.cameraman = str;
			if (txt.readLine(str))d.medal = Parse<int>(str);
		}
	}

	void Save() {
		TextWriter writer(file);

		writer << gamepadNum[0];
		writer << gamepadNum[1];
		writer << gamepadNum[2];
		writer << gamepadNum[3];
		writer << bgmVolume;
		writer << soundVolume;
		writer << windowSize;
		writer << hardMode;
		writer << shotMode;

		for (auto& d : data) {
			writer << d.hiScore;
			writer << d.total;
			writer << d.score;
			writer << d.cameraman;
			writer << d.medal;
		}
	}
};