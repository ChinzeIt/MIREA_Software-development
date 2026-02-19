#pragma once

#include <string>

class createFileService {
    public:
    virtual ~createFileService() = default;
    virtual bool createFile(const std::string& rawPath) = 0;
};
