#ifndef __JSONFILE_H__
#define __JSONFILE_H__

#include "json.hpp"

using json = nlohmann::ordered_json;

class JsonFile
{
public:
	JsonFile();
	~JsonFile();

	bool Load(const char* path);
	bool Save(const char* path);

public:
	json file;
};

#endif //__CONFIG_H__