#include "qtCheckCreateService.h"

#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>

bool QTCheckCreateService::checking(const std::string& rawPath) {
    path = FileCreateValidationResult{};

    QString FrawPath = QString::fromStdString(rawPath).trimmed();

    path.empty = !FrawPath.isEmpty();
    if (!path.empty) return false;

    QFileInfo info(FrawPath);

    QDir parentDir = info.dir();
    path.parentDirExists = parentDir.exists();

    QDir cwd = QDir::current();
    QString absPath = info.absoluteFilePath();
    QString absCwd  = cwd.absolutePath();
    path.insideWorkingDir = absPath == absCwd || absPath.startsWith(absCwd + QDir::separator());

    path.hasTraversal = !FrawPath.contains("..");

    QFileInfo parentInfo(parentDir.absolutePath());

    path.writable = parentInfo.exists() && parentInfo.isWritable();

    path.noFileExists = !info.exists();

    QRegularExpression invalidChars(R"([<>:"/\\|?*\x00-\x1f])");
    path.validName = !info.fileName().isEmpty() && !invalidChars.match(info.fileName()).hasMatch();
    path.isSystemLink = !(rawPath == "." || rawPath == "..");

    return path.empty &&
            path.insideWorkingDir &&
            path.parentDirExists &&
            path.hasTraversal &&
            path.writable &&
            path.noFileExists &&
            path.validName;
}

std::string QTCheckCreateService::error() {
    std::string errorText;

    errorText.append("Error: ");

    for (auto [name, ptr] : path.get_fields())
        if (!*ptr) errorText.append(name).append(", ");

    if (errorText.size() >= 2) errorText.resize(errorText.size() - 2);
    
    return errorText;
}