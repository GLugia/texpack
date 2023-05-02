#include "FileStream.h"

BinaryReader::BinaryReader()
	: file_length(-1) { }

void BinaryReader::Open(Path path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::exception(("File not found: \"" + path.string() + "\"").c_str());
	}
	this->stream.open(path, FileMode::BINARY | FileMode::READ);
	if (this->stream.bad() || this->stream.fail())
	{
		throw std::exception(("Failed to open file \"" + path.string() + "\"").c_str());
	}
	this->stream.seekg(0, FileDirection::END);
	this->file_length = (s64)this->stream.tellg();
	this->stream.seekg(0, FileDirection::BEGIN);
}

s64 BinaryReader::GetLength()
{
	return this->file_length;
}

s64 BinaryReader::GetPosition()
{
	return (s64)this->stream.tellg();
}

void BinaryReader::Seek(s64 offset, FileDirection direction)
{
	this->stream.seekg(offset, direction);
}

c8 BinaryReader::ReadC8()
{
	return this->Read<c8>();
}

c16 BinaryReader::ReadC16()
{
	return this->Read<c16>();
}

c32 BinaryReader::ReadC32()
{
	return this->Read<c32>();
}

c64 BinaryReader::ReadC64()
{
	return this->Read<c64>();
}

u8 BinaryReader::ReadU8()
{
	return this->Read<u8>();
}

u16 BinaryReader::ReadU16()
{
	return this->Read<u16>();
}

u32 BinaryReader::ReadU32()
{
	return this->Read<u32>();
}

u64 BinaryReader::ReadU64()
{
	return this->Read<u64>();
}

s8 BinaryReader::ReadS8()
{
	return this->Read<s8>();
}

s16 BinaryReader::ReadS16()
{
	return this->Read<s16>();
}

s32 BinaryReader::ReadS32()
{
	return this->Read<s32>();
}

s64 BinaryReader::ReadS64()
{
	return this->Read<s64>();
}

f32 BinaryReader::ReadF32()
{
	return this->Read<f32>();
}

f64 BinaryReader::ReadF64()
{
	return this->Read<f64>();
}

string BinaryReader::ReadString()
{
	string result{ };
	while (this->stream.peek())
	{
		result += this->ReadC8();
	}
	return result;
}

void BinaryReader::Align(u64 to_size)
{
	auto mod = this->GetPosition() % to_size;
	if (mod != 0)
	{
		this->Seek(to_size - mod, FileDirection::CURRENT);
	}
}

void BinaryReader::Close()
{
	this->stream.close();
	this->file_length = -1;
}

BinaryWriter::BinaryWriter() { }

void BinaryWriter::Open(Path path)
{
	this->stream.open(path, FileMode::BINARY | FileMode::WRITE);
	if (this->stream.bad() || this->stream.fail())
	{
		throw std::exception(("Failed to open file \"" + path.string() + "\"").c_str());
	}
}

s64 BinaryWriter::GetLength()
{
	s64 cur_pos = (s64)this->stream.tellp();
	this->stream.seekp(0, FileDirection::END);
	s64 result = (s64)this->stream.tellp();
	this->stream.seekp(cur_pos, FileDirection::BEGIN);
	return result;
}

s64 BinaryWriter::GetPosition()
{
	return (s64)this->stream.tellp();
}

void BinaryWriter::Write(string str)
{
	this->stream.write(str.data(), str.size());
}

void BinaryWriter::Flush()
{
	this->stream.flush();
}

void BinaryWriter::Align(const s64 to_size)
{
	std::vector<c8> buffer{};
	buffer.resize(to_size);
	this->stream.write(buffer.data(), to_size);
}

void BinaryWriter::Close()
{
	this->stream.close();
}
