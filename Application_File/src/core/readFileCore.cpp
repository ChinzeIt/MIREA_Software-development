#include "readFileService.h"

#include <fstream>
#include <sstream>
#include <string>

class STDReadFileService : public readFileService {
    std::string readFile(const std::string& rawPath) override {
        std::string result = "Can not open file or content have damage";

        std::ifstream file(rawPath, std::ios::in);
        if (!file.is_open())
            return result;

        result.clear();

        std::ostringstream ss;
        ss << file.rdbuf();
        result = ss.str();

        return result;
    }
};