// Connect Texture Packer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Global.h"
#include "TexturePacker.h"

c32 main(c32 argc, c8* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cout << "Arguments were: ";
			for (c32 i = 0; i < argc; i++)
			{
				if (i)
				{
					std::cout << " ";
				}
				std::cout << argv[i];
			}
			std::cout << std::endl;
			throw std::exception("Invalid arguments. Try again with -help");
		}

		TexturePacker packer{};

		const string arg = argv[1];
		const string unpack = "-unpack";
		const string pack = "-pack";
		const string help = "-help";

		if (arg == unpack && argc == 3)
		{
			Path path = argv[2];
			packer.Read(path);
			path = std::filesystem::current_path() / Path("output") / path.filename().replace_extension();
			std::filesystem::create_directory(path);
			packer.Dump(path);
		}
		else if (arg == pack && argc == 3)
		{
			Path path = argv[2];
			packer.Gather(path);
			Path temp = path.filename();
			path = std::filesystem::current_path() / "output";
			std::filesystem::create_directories(path);
			path = path / temp;
			path += Path(".texpack");
			packer.Write(path);
		}
		else if (arg == help)
		{
			std::cout << "Format: -[unpack, pack] [target]" << std::endl;
			std::cout << "-unpack [texpack] : Unpacks all images from the given .texpack file." << std::endl;
			std::cout << "-pack [directory] : Packs all files contained in the given directory into a single .texpack file." << std::endl;
			std::cout << "\tResulting files will be placed in an /output/ folder relative to this executable's location." << std::endl;
		}
		else
		{
			std::cout << "Arguments were: ";
			for (c32 i = 0; i < argc; i++)
			{
				if (i)
				{
					std::cout << " ";
				}
				std::cout << argv[i];
			}
			std::cout << std::endl;
			throw std::exception("Invalid arguments. Try again with -help");
		}
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
