#pragma once
#include "checkCreate.h"

#include <string>

class checkCreateService {
    public:
    virtual ~checkCreateService() = default;

    virtual  bool checking(const std::string& rawPath) = 0;

    virtual std::string error() = 0;
};