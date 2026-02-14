#pragma once

#include "../../core/readFileService.h"

#include <QString>
#include <QFile>
#include <QTextStream>

class QTReadFileService : public readFileService {
    public:
    std::string readFile(const std::string& rawPath) override;
};