#pragma once

#include "../../core/checkPathService.h"

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QString>
#include <string>

class QTCheckPathService: public checkPathService {
    private:
    PathValidationResult path;
    public:
    bool checking (const std::string& rawPath) override;
    std::string error() override;
};