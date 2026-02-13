#pragma once

#include "../../core/diskService.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/statvfs.h>

class LinuxDiskService : public DiskService {
    private:
    std::vector<DiskInfo> disk;
    std::string infoLine;

    public:
    void setDisk () override;
    std::vector<DiskInfo> getDisk () override;

    void setInfoLine () override;
    std::string getInfoLine () override;
};