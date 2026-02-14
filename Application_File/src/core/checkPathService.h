#pragma once

#include "checkPath.h"
#include <string>

class checkPathService {
    public:
    virtual ~checkPathService() = default;

    virtual  bool checking(const std::string& rawPath) = 0;

    virtual std::string error() = 0;
};