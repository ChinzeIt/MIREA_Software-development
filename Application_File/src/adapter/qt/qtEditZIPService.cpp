#include "qtEditZIPService.h"

void QTEditZIPService::delFile(const std::vector<std::string>& vec) {
    if (vec.empty()) return;

    QSet<QString> toDelete;
    for (const auto& path : vec)
        toDelete.insert(QString::fromStdString(path));

    QString filePath = file.fileName();
    QString tempPath = filePath + ".tmp";

    QuaZip srcZip(filePath);
    if (!srcZip.open(QuaZip::mdUnzip))
        throw std::runtime_error("cannot open source zip");

    int total = 0;
    for (bool has = srcZip.goToFirstFile(); has; has = srcZip.goToNextFile())
        total++;

    QLockFile tempLock(tempPath + ".lock");
    if (!tempLock.tryLock()) {
        srcZip.close();
        throw std::runtime_error("cannot lock temp file");
    }

    QuaZip dstZip(tempPath);
    if (!dstZip.open(QuaZip::mdCreate)) {
        srcZip.close();
        tempLock.unlock();
        QFile::remove(tempPath);
        throw std::runtime_error("cannot create temp zip");
    }

    int current = 0;
    for (bool has = srcZip.goToFirstFile(); has; has = srcZip.goToNextFile()) {
        QString entryName = srcZip.getCurrentFileName();

        bool skip = toDelete.contains(entryName);
        if (!skip) {
            for (const QString& del : toDelete) {
                if (entryName.startsWith(del.endsWith('/') ? del : del + '/')) {
                    skip = true;
                    break;
                }
            }
        }

        if (!skip) {
            QuaZipFile srcFile(&srcZip);
            srcFile.open(QIODevice::ReadOnly);

            QuaZipFile dstFile(&dstZip);
            dstFile.open(QIODevice::WriteOnly, QuaZipNewInfo(entryName));
            dstFile.write(srcFile.readAll());

            srcFile.close();
            dstFile.close();
        }

        if (onProgress && total > 0)
            onProgress(++current * 100 / total);
    }

    srcZip.close();
    dstZip.close();
    file.close();
    tempLock.unlock();
    QFile::remove(tempPath + ".lock");

    if (!QFile::remove(filePath)) {
        QFile::remove(tempPath);
        throw std::runtime_error("cannot remove original zip");
    }

    if (!QFile::rename(tempPath, filePath)) {
        QFile::remove(tempPath);
        throw std::runtime_error("cannot rename temp zip");
    }

    if (!file.open(QIODevice::ReadWrite))
        throw std::runtime_error("cannot reopen file after delete");
}

void QTEditZIPService::comFile(std::string rawPath) {
    QString filePath = file.fileName();
    QString sourcePath = QString::fromStdString(rawPath);
    QFileInfo sourceInfo(sourcePath);

    if (!sourceInfo.exists() || !sourceInfo.isFile()) {
        throw std::runtime_error("source file does not exist or is not a file");
    }

    QuaZip zip(filePath);
    if (!zip.open(QuaZip::mdAdd)) {
        throw std::runtime_error("cannot open zip for adding");
    }

    QuaZipFile zipFile(&zip);
    QuaZipNewInfo info(sourceInfo.fileName());

    if (!zipFile.open(QIODevice::WriteOnly, info)) {
        zip.close();
        throw std::runtime_error("cannot create entry in zip");
    }

    QFile source(sourcePath);
    if (!source.open(QIODevice::ReadOnly)) {
        zipFile.close();
        zip.close();
        throw std::runtime_error("cannot open source file");
    }

    const qint64 chunkSize = 64 * 1024;
    const qint64 totalSize = sourceInfo.size();

    while (!source.atEnd()) {
        zipFile.write(source.read(chunkSize));
        if (onProgress && totalSize > 0) {
            onProgress(source.pos() * 100 / totalSize);
        }
    }

    source.close();
    zipFile.close();
    zip.close();
}