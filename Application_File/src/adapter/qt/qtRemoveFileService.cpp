#include "qtRemoveFileService.h"

bool QTRemoveFileService::checkBlock (const std::string& strPath) {
    QString path = QString::fromStdString(strPath);
    QLockFile lock(path + ".lock");

    if (lock.tryLock(0)) {
        lock.unlock();
        return true;
    }

    return false;
}

void QTRemoveFileService::deleteFile (const std::string& strPath) {
    QString path = QString::fromStdString(strPath);

    QFile file(path);
    if (!file.remove())
        throw std::runtime_error("cannot remove file");
}