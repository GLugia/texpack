#pragma once

#include "Global.h"
#include <ios>
#include <filesystem>
#include <fstream>
#include <istream>
#include <ostream>

enum FileMode
{
	READ = std::ios::in,
	WRITE = std::ios::out,
	APPEND = std::ios::app,
	TRUNCATE = std::ios::trunc,

	BINARY = std::ios::binary,
	HEX = std::ios::hex,
	HEX_FLOAT = std::ios::hexfloat
};

enum FileDirection
{
	BEGIN = std::ios::beg,
	CURRENT = std::ios::cur,
	END = std::ios::end
};

using Path = std::filesystem::path;

class BinaryReader
{
private:
	std::fstream stream;
	s64 file_length;
public:
	BinaryReader();
	void Open(Path path);
	s64 GetLength();
	s64 GetPosition();
	void Seek(s64 offset, FileDirection direction);
	template<typename _Type>
	inline _Type Read()
	{
		_Type type{};
		this->stream.read((c8*)&type, sizeof(_Type));
		return type;
	}
	template<typename _Type>
	inline void Read(_Type* ptr, c32 length)
	{
		this->stream.read((c8*)ptr, sizeof(_Type) * length);
	}
	c8 ReadC8();
	c16 ReadC16();
	c32 ReadC32();
	c64 ReadC64();
	u8 ReadU8();
	u16 ReadU16();
	u32 ReadU32();
	u64 ReadU64();
	s8 ReadS8();
	s16 ReadS16();
	s32 ReadS32();
	s64 ReadS64();
	f32 ReadF32();
	f64 ReadF64();
	string ReadString();
	void Align(u64 to_size);
	void Close();
};

class BinaryWriter
{
private:
	std::fstream stream;
public:
	BinaryWriter();
	void Open(Path path);
	s64 GetLength();
	s64 GetPosition();
	void Write(string str);
	template<typename _Type>
	inline void Write(_Type type)
	{
		this->stream.write((c8*)&type, sizeof(_Type));
	}
	template<typename _Type>
	inline void Write(_Type* ptr, c32 length)
	{
		this->stream.write((c8*)ptr, sizeof(_Type) * length);
	}
	void Flush();
	void Align(const s64 to_size);
	void Close();
};
