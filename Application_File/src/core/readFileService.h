#pragma once

#include <string>

class readFileService {
    public:
    virtual ~readFileService() = default;
    virtual std::string readFile(const std::string& rawPath) = 0;
};