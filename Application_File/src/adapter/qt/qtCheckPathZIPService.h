#pragma once

#include "../../core/checkPathZIPService.h"

#include <QFileInfo>
#include <QDir>
#include <QFile>

class QTCheckPathZIPService : public CheckPathZIPService {
    private:
    PathValidationResZIP path;
    public:
    bool checking(const std::string& rawPath, PathAccessModeZIP  mode) override;

    std::string error() override;
};