#pragma once

#include "createZIPService.h"

#include <fstream>
#include <array>

class STDCreateZIPService : public createZIPService {
    public:
    bool createZIP (const std::string& rawPath) override;
};