#pragma once

#include "../../core/removeFileService.h"

#include <QString>
#include <QLockFile>
#include <QFile>

class QTRemoveFileService : public RemoveFileService {
    public:
    bool checkBlock (const std::string& strPath) override;
    void deleteFile (const std::string& strPath) override;
};