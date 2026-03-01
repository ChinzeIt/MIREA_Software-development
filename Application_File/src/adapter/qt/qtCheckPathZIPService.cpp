#include "qtCheckPathZIPService.h"


bool QTCheckPathZIPService::checking(const std::string& rawPath, PathAccessModeZIP mode) {
    path = PathValidationResZIP{};

    QString FrawPath = QString::fromStdString(rawPath).trimmed();
    if (FrawPath.isEmpty()) {
        path.empty = false;
        return false;
    }

    QString cleanPath = QDir::cleanPath(FrawPath);
    QFileInfo info(cleanPath);

    path.exists       = info.exists();
    path.isFile       = info.isFile();
    path.hasExtension = !info.suffix().isEmpty();

    QFileInfo parentInfo(info.dir().absolutePath());
    path.dirExecutable = parentInfo.permission(QFile::ExeUser);
    path.userReadable  = info.permission(QFile::ReadUser);
    path.hasTraversal  = !cleanPath.contains("..");

    if (info.exists()) {
        QFileInfo canonical(info.canonicalFilePath());
        path.canonicalizable = canonical.exists();
    }

    QDir cwd = QDir::current();
    QString absPath = info.absoluteFilePath();
    QString absCwd  = cwd.absolutePath();
    path.insideWorkingDir =
        absPath == absCwd ||
        absPath.startsWith(absCwd + QDir::separator());

    path.zipExtension = info.suffix().compare("zip", Qt::CaseInsensitive) == 0;

    if (info.exists() && info.isFile()) {
        QFile file(info.absoluteFilePath());
        if (file.open(QIODevice::ReadOnly)) {
            path.zipReadable = true;
            QByteArray sig = file.read(4);
            path.zipSignature =
                sig.startsWith("PK\x03\x04") ||
                sig.startsWith("PK\x05\x06") ||
                sig.startsWith("PK\x07\x08");
            path.zipNotCorrupted = path.zipSignature;
            file.close();
        } else {
            path.zipReadable     = false;
            path.zipSignature    = false;
            path.zipNotCorrupted = false;
        }
    }

    switch (mode) {
        case PathAccessModeZIP::COMPRESS:
            path.readable = info.isReadable();
            path.noDuplicate = !existingEntries.contains(info.fileName());
            break;
        case PathAccessModeZIP::EDIT:
            path.readable = info.isReadable();
            path.writable = info.isWritable();
            break;
        case PathAccessModeZIP::EXTRACT: {
            path.writable =
                parentInfo.exists() &&
                parentInfo.permission(QFile::WriteUser);
            break;
        }
        default: break;
    }

    return
        path.empty &&
        path.dirExecutable &&
        path.userReadable &&
        path.hasTraversal &&
        (
            (mode == PathAccessModeZIP::CONTENT &&
                path.isFile &&
                path.hasExtension &&
                path.canonicalizable &&
                path.zipExtension &&
                path.exists &&
                path.readable &&
                path.zipReadable &&
                path.zipSignature &&
                path.zipNotCorrupted
            ) ||
            (mode == PathAccessModeZIP::EDIT &&
                path.isFile &&
                path.hasExtension &&
                path.canonicalizable &&
                path.zipExtension &&
                path.exists &&
                path.readable &&
                path.writable &&
                path.zipReadable &&
                path.zipSignature &&
                path.zipNotCorrupted
            ) ||
            (mode == PathAccessModeZIP::EXTRACT &&
                path.writable
            ) ||
            (mode == PathAccessModeZIP::COMPRESS &&
                path.exists &&
                path.isFile &&
                path.readable &&
                path.noDuplicate
            )
        );
}

std::string QTCheckPathZIPService::error() {
    std::string errorText;

    errorText.append("Error: ");

    for (auto [name, ptr] : path.get_fields())
        if (!*ptr) errorText.append(name).append(", ");

    if (errorText.size() >= 2) errorText.resize(errorText.size() - 2);
    
    return errorText;
}