#pragma once

#include <Siv3D.hpp>

namespace ph {
	class BinaryStreamR : public AngelScript::asIBinaryStream {
		BinaryReader br;
	public:
		BinaryStreamR(String filePath):br(filePath){}

		int Read(void* ptr, AngelScript::asUINT size)override {
			return br.read(ptr, size) ? 0 : -1;
		}

		int Write(const void* ptr, AngelScript::asUINT size)override {
			assert(0);
			return -1;
		}
	};

	class BinaryStreamW :public AngelScript::asIBinaryStream {
		BinaryWriter bw;
	public:
		BinaryStreamW(String filePath) :bw(filePath) {}

		int Read(void* ptr, AngelScript::asUINT size)override {
			assert(0);
			return -1;
		}

		int Write(const void* ptr, AngelScript::asUINT size)override {
			return bw.write(ptr, size) ? 0 : -1;;
		}
	};
}