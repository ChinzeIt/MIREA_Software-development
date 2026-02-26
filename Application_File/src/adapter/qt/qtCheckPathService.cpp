#include "qtCheckPathService.h"

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

bool QTCheckPathService::checking (const std::string& rawPath, PathAccessMode mode) {
    path = PathValidationResult{};

    QString FrawPath = QString::fromStdString(rawPath);

    if (FrawPath.trimmed().isEmpty()) {
        path.empty = false;
        return false;
    }

    QString cleanPath = QDir::cleanPath(FrawPath);
    QFileInfo info(cleanPath);

    path.exists   = info.exists();
    path.isFile   = info.isFile();

    path.hasExtension = !info.suffix().isEmpty();

    path.absolute = !info.isAbsolute();

    QFileInfo canonicalInfo;
    if (info.exists()) {
        canonicalInfo.setFile(info.canonicalFilePath());
        path.canonicalizable = canonicalInfo.exists();
    }

    path.isSymlink = !info.isSymLink();
    if (path.isSymlink) {
        QFileInfo target(info.symLinkTarget());
        path.symlinkResolved = !target.exists();
    }

    QDir parentDir = info.dir();
    path.parentDirExists = parentDir.exists();

    QDir cwd = QDir::current();
    QString absPath = info.absoluteFilePath();
    QString absCwd  = cwd.absolutePath();

    path.hasTraversal = !cleanPath.contains("..");

    path.insideWorkingDir =
        absPath == absCwd ||
        absPath.startsWith(absCwd + QDir::separator());

    switch (mode) {
        case PathAccessMode::Create: {
            path.noFileExists = !info.exists();
            QRegularExpression invalidChars(R"([<>:"/\\|?*\x00-\x1f])");
            path.validName = !info.fileName().isEmpty() && !invalidChars.match(info.fileName()).hasMatch();
            path.isSystemLink = !(rawPath == "." || rawPath == "..");
            break;
        }
        case PathAccessMode::Read:
            path.readable = info.isReadable();
            path.userReadable = info.permission(QFile::ReadUser);
            break;
        case PathAccessMode::Write:
            path.readable = info.isReadable();
            path.writable = info.isWritable();
            path.userWritable = info.permission(QFile::WriteUser);
            break;
        case PathAccessMode::Remove: {
            QFileInfo parentInfo(parentDir.absolutePath());
            path.removable =
                parentInfo.exists() &&
                parentInfo.permission(QFile::WriteUser);
            break;
        }
    }

    switch (mode) {
    case PathAccessMode::Create:
        return path.empty &&
            path.noFileExists &&
            path.validName &&
            path.isSystemLink &&
            path.hasExtension &&
            path.parentDirExists &&
            path.insideWorkingDir &&
            path.hasTraversal;

    case PathAccessMode::Read:
        return path.empty &&
            path.exists &&
            path.isFile &&
            path.readable &&
            path.userReadable &&
            path.insideWorkingDir &&
            path.hasTraversal;

    case PathAccessMode::Write:
        return path.empty &&
            path.exists &&
            path.isFile &&
            path.writable &&
            path.userWritable &&
            path.insideWorkingDir &&
            path.hasTraversal;

    case PathAccessMode::Remove:
        return path.empty &&
            path.exists &&
            path.isFile &&
            path.removable &&
            path.insideWorkingDir &&
            path.hasTraversal;
    }
    return false;
}

std::string QTCheckPathService::error() {
    std::string errorText;

    errorText.append("Error: ");

    for (auto [name, ptr] : path.get_fields())
        if (!*ptr) errorText.append(name).append(", ");

    if (errorText.size() >= 2) errorText.resize(errorText.size() - 2);
    
    return errorText;
}
