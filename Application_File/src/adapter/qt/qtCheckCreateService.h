#pragma once

#include "../../core/checkCreateService.h"

class QTCheckCreateService : public checkCreateService {
    private:
    FileCreateValidationResult path;
    public:
    bool checking(const std::string& rawPath) override;
    std::string error() override;
};