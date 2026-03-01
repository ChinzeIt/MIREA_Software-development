#pragma once

#include <string>
#include <vector>

class EditZIPService {
    public: // RAII must realization block and unblock file when ZIP edit
    virtual ~EditZIPService() = default;
    virtual void delFile(const std::vector<std::string>& vec) = 0;
    virtual void comFile(std::string) = 0;
};