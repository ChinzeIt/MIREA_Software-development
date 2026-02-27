#pragma once

#include <string>

class RemoveFileService {
    public:
    virtual ~RemoveFileService() = default;

    virtual bool checkBlock (const std::string& strPath) = 0;
    virtual void deleteFile (const std::string& strPath) = 0;
};