#include "FileManager.h"

std::string FileManager::LoadFile(std::string fileName)
{
    std::ifstream inFile(fileName, std::ios::binary);
    std::string str;

    if (inFile.is_open()) {
        str.clear();
        // allocate string data (no slower realloc)
        inFile.seekg(0, std::ios::end);
        str.reserve(inFile.tellg());
        inFile.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
        inFile.close();
    }
    else {
       // Logger::log(1, "%s error: could not open file %s\n", __FUNCTION__, fileName.c_str());
      //  Logger::log(1, "%s error: system says '%s'\n", __FUNCTION__, strerror(errno));
        return std::string();
    }

    if (inFile.bad() || inFile.fail()) {
      //  Logger::log(1, "%s error: error while reading file %s\n", __FUNCTION__, fileName.c_str());
        inFile.close();
        return std::string();
    }

    inFile.close();
   // Logger::log(1, "%s: file %s successfully read to string\n", __FUNCTION__, fileName.c_str());
    return str;
}
