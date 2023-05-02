#pragma once

#include "Global.h"
#include "FileStream.h"

struct Image
{
	c8* name;
	c8* data;
	c32 size;
};

struct ImageHeader
{
	c8 name_length;
	c8 name_padding;
	c8 data_padding;
	c8 terminator; // possibly unused
};

class TexturePacker
{
private:
	std::vector<Image> images;
public:
	TexturePacker();

	void Read(Path from_texpack);
	void Dump(Path to_directory);

	void Gather(Path from_directory);
	void Write(Path to_texpeck);

	~TexturePacker();
};

