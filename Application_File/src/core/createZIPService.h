#pragma once

#include <string>

class createZIPService {
    public:
    virtual ~createZIPService() = default;
    virtual bool createZIP(const std::string& rawPath) = 0;
};
