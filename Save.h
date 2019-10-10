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

		for (auto& d : data) {
			writer << d.hiScore;
			writer << d.total;
			writer << d.score;
			writer << d.cameraman;
			writer << d.medal;
		}
	}
};