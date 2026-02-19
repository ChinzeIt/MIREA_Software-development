#include "qtCreateFileService.h"

bool QTCreateFileService::createFile(const std::string& rawPath) {
    QString qPath = QString::fromStdString(rawPath);

    QFileInfo info(qPath);


    QFile file(qPath);
    if (!file.open(QIODevice::WriteOnly)) {
        file.close();
        return false;
    }

    file.close();

    return true;
}