#include "qtReadFileService.h"

std::string QTReadFileService::readFile (const std::string& rawPath) {
    QString QrawPath = QString::fromStdString(rawPath);

#if defined(Q_OS_WIN)
    return fixWindowsRead(rawPath);
#else
    QFile file(QrawPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "Can not open file or content have damage";

    QTextStream in(&file);
    return in.readAll().toStdString();
#endif
}
// Not tested on OS "Windows"
std::string QTReadFileService::fixWindowsRead(const std::string& rawPath) {
    QString QrawPath = QString::fromStdString(rawPath);
    std::string result = "Cannot open file or content is damaged";

    QFileInfo fileInfo(QrawPath);
    if (!fileInfo.exists() || !fileInfo.isFile()) return result;

    QTemporaryFile tempFile(fileInfo.dir().absoluteFilePath("XXXXXX_temp.txt"));
    if (!tempFile.open()) return result;

    if (!QFile::copy(QrawPath, tempFile.fileName())) return result;

    QTextStream in(&tempFile);
    result.clear();
    result = in.readAll().toStdString();

    tempFile.close();
    tempFile.remove();

    return result;
}