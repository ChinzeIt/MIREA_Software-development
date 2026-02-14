#include "qtReadFileService.h"

std::string QTReadFileService::readFile (const std::string& rawPath) {
    QString QrawPath = QString::fromStdString(rawPath);
    std::string result = "Can not open file or content have damage";

    QFile file(QrawPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return result;

    result.clear();
    QTextStream in(&file);
    result = in.readAll().toStdString();

    return result;
}