#pragma once

#include "../../core/readFileService.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>
#include <QTemporaryFile>

class QTReadFileService : public readFileService {
    public:
    std::string readFile(const std::string& rawPath) override;
    std::string fixWindowsRead(const std::string& rawPath); // WARNING!!!
};