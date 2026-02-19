#pragma once

#include "../../core/createFileService.h"

#include <QFile>
#include <QFileInfo>
#include <QString>

class QTCreateFileService : public createFileService {
    public:
    bool createFile(const std::string& rawPath) override;
};