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

class FileManager
{
public:
	static std::string LoadFile(std::string fileName);

private:

};

