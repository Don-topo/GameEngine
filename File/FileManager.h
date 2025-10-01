#pragma once

#include "../Logger/Logger.h"
#include <string>
#include <optional>
#include <iostream>
#include <fstream>
#include <cerrno>  // errno
#include <cstring> // strerror()
#include <limits>
#include <filesystem>
#include <yaml-cpp/yaml.h>

class FileManager
{
public:
	static std::string LoadFile(std::string fileName);
	static void LoadYamlFile(std::string filename);

	void CreateYamlFile();
	void SaveYamlFile(std::string fileName);

	bool HasKey(std::string key);
	void GetValue(std::string key, std::string& value);
private:
	std::string yamlFileName;
	YAML::Node yamlNode = {};
	YAML::Emitter yamlEmitter = {};
};

