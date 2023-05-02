#include "TexturePacker.h"
#include <exception>
#include <vector>

TexturePacker::TexturePacker() { }

void TexturePacker::Read(Path from_texpack)
{
	const s64 alignment = 16;

	BinaryReader reader{};
	reader.Open(from_texpack);

	c32 count = reader.ReadU32();
	std::cout << "Attempting to read " << std::to_string(count) << " files..." << std::endl;
	reader.Align(alignment);

	std::vector<ImageHeader> headers{};

	for (c32 i = 0; i < count; i++)
	{
		headers.push_back
		(
			ImageHeader
			{
				.name_length = reader.ReadC8(),
				.name_padding = reader.ReadC8(),
				.data_padding = reader.ReadC8(),
				.terminator = reader.ReadC8()
			}
		);
		this->images.push_back
		(
			Image
			{
				.name = nullptr,
				.data = nullptr,
				.size = reader.ReadC32()
			}
		);
	}

	reader.Align(alignment);

	for (c32 i = 0; i < count; i++)
	{
		this->images[i].name = (c8*)std::malloc(headers[i].name_length);
		reader.Read(this->images[i].name, headers[i].name_length);
		std::cout << "  Reading " << this->images[i].name << " at " << std::hex << std::to_string(reader.GetPosition()) << "...";
		reader.Seek(headers[i].name_padding, FileDirection::CURRENT);
		this->images[i].data = (c8*)std::malloc(this->images[i].size);
		reader.Read(this->images[i].data, this->images[i].size);
		reader.Seek(headers[i].data_padding, FileDirection::CURRENT);
		std::cout << "Done!" << std::endl;
	}

	if (reader.GetPosition() < reader.GetLength() - 1)
	{
		throw std::exception("Did not read all available data!");
	}

	reader.Close();

	std::cout << "Done!" << std::endl;
}

void TexturePacker::Dump(Path to_directory)
{
	std::cout << "Attempting to write " << std::to_string(this->images.size()) << " files..." << std::endl;
	for (Image& image : this->images)
	{
		std::cout << "  Writing " << image.name << "...";
		BinaryWriter writer{};
		writer.Open(to_directory / image.name);
		writer.Write(image.data, image.size);
		writer.Flush();
		writer.Close();
		std::cout << "Done!" << std::endl;
	}
	std::cout << "Done!" << std::endl;
}

void TexturePacker::Gather(Path from_directory)
{
	std::cout << "Loading files from \"" << from_directory.string() << "\"..." << std::endl;
	std::filesystem::directory_iterator iterator{ from_directory };
	for (const auto& entry : iterator)
	{
		if (entry.is_regular_file())
		{
			Path path = entry.path();
			std::cout << "  Reading \"" << path.string() + "\"...";

			string file_name = path.filename().string();
			if (file_name.length() > 16)
			{
				throw std::exception(("File names (including extension) must be 16 characters or less: \"" + file_name + "\"").c_str());
			}

			
			Image image{};

			// copy file name
			image.name = (c8*)std::malloc(file_name.size());
			std::strcpy(image.name, file_name.c_str());

			// get the size of the file
			image.size = (c32)std::filesystem::file_size(path);

			// copy file data
			image.data = (c8*)std::malloc(image.size);
			BinaryReader reader{};
			reader.Open(path);
			reader.Read(image.data, image.size);
			reader.Close();

			this->images.push_back(image);
			std::cout << "Done!" << std::endl;
		}
	}
	if (!this->images.size())
	{
		throw std::exception(("No files found in given directory: \"" + from_directory.string() + "\"").c_str());
	}
	std::cout << "Done!" << std::endl;
}

void TexturePacker::Write(Path to_texpack)
{
	const u64 alignment = 16;
	std::cout << "Writing images to \"" + to_texpack.string() + "\"..." << std::endl;
	BinaryWriter writer{};
	writer.Open(to_texpack);
	// write the number of files contained and align it to 16
	writer.Write((c32)this->images.size());
	writer.Align(alignment - sizeof(c32));
	// writer the metadata for each file
	for (Image& image : this->images)
	{
		std::cout << "  " << image.name << " header...";
		u8 len = (u8)(std::strlen(image.name) + 1);
		writer.Write((u8)len);
		writer.Write((u8)(alignment - len));
		writer.Write((u8)(alignment - (image.size % alignment)));
		writer.Write((u8)0xCC);
		writer.Write((s32)image.size);
		std::cout << "Done!" << std::endl;
	}
	if (writer.GetPosition() % alignment != 0)
	{
		writer.Align(alignment - (writer.GetPosition() % alignment));
	}
	for (Image& image : this->images)
	{
		std::cout << "  " << image.name << " data...";
		u8 len = (u8)std::strlen(image.name);
		writer.Write(image.name, len);
		writer.Align(alignment - len);
		writer.Write(image.data, image.size);
		writer.Align(alignment - (image.size % alignment));
		std::cout << "Done!" << std::endl;
	}
	writer.Flush();
	writer.Close();
	std::cout << "Done!" << std::endl;
}

TexturePacker::~TexturePacker() { }
