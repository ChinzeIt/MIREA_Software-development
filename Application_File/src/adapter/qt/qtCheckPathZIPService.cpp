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

    path.exists = info.exists();
    path.isFile = info.isFile();
    path.hasExtension = !info.suffix().isEmpty();

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
            path.zipReadable = false;
            path.zipSignature = false;
            path.zipNotCorrupted = false;
        }
    }

    switch (mode) {
        case PathAccessModeZIP::CONTENT:
            path.readable = info.isReadable();
            break;

        case PathAccessModeZIP::EDIT:
            path.readable = info.isReadable();
            path.writable = info.isWritable();
            break;

        case PathAccessModeZIP::UPLOAD:
            path.exists = !info.exists() || info.exists();
            QDir parentDir = info.dir();
            QFileInfo parentInfo(parentDir.absolutePath());

            path.writable =
                parentInfo.exists() &&
                parentInfo.permission(QFile::WriteUser);
            break;
    }

    return
        path.empty &&
        path.isFile &&
        path.hasExtension &&
        path.canonicalizable &&
        path.insideWorkingDir &&
        path.zipExtension &&
        (
            (mode == PathAccessModeZIP::CONTENT &&
                path.exists &&
                path.readable &&
                path.zipReadable &&
                path.zipSignature &&
                path.zipNotCorrupted
            ) ||
            (mode == PathAccessModeZIP::EDIT &&
                path.exists &&
                path.readable &&
                path.writable &&
                path.zipReadable &&
                path.zipSignature &&
                path.zipNotCorrupted
            ) ||
            (mode == PathAccessModeZIP::UPLOAD &&
                path.writable
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