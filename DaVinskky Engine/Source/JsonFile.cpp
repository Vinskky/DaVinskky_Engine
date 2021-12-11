#include "JsonFile.h"
#include "Globals.h"

#include <fstream>

JsonFile::JsonFile()
{}

JsonFile::~JsonFile()
{}

bool JsonFile::Load(const char* path)
{
	std::ifstream input(path);
	if (input.is_open())
	{
		input >> file;
		return true;
	}
	else
	{
		LOG("Error while loading json file %s", path);
		return false;
	}

	input.close();
}

bool JsonFile::Save(const char* path)
{
	std::ofstream output(path);
	if (output.is_open())
	{
		output << std::setw(4) << file << std::endl;
		return true;
	}
	else
	{
		LOG("Error while saving json file %s", path);
		return false;
	}

	output.close();
}