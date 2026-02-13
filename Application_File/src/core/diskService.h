#pragma once

#include <string>
#include <vector>

#include "diskInfo.h"

class DiskService {
    public:
    virtual ~DiskService() = default;

    virtual void setDisk() = 0;
    virtual std::vector<DiskInfo> getDisk() = 0;

    virtual void setInfoLine() = 0;
    virtual std::string getInfoLine() = 0;
};