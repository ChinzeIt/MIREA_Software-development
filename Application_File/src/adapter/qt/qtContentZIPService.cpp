#include "qtContentZIPService.h"

void QTContentZIPService::loadContent(std::string zipPath) {
    c.clear();
    QString qZipPath = QString::fromStdString(zipPath);

    QuaZip zip(qZipPath);
    if (!zip.open(QuaZip::mdUnzip)) {
        content error;
        error.path="Can not open file";
        error.uncompressSize=0;
        error.datatime="0";
        c.push_back(error);
        return;
    }

    QuaZipFile file(&zip);

    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        QuaZipFileInfo info;
        if (!zip.getCurrentFileInfo(&info)) {
            content error;
            error.path="Can get info about file";
            error.uncompressSize=0;
            error.datatime="0";
            c.push_back(error);
            continue;
        }

        content entry;
        entry.path = info.name.toStdString();
        entry.uncompressSize = static_cast<long long>(info.uncompressedSize);

        QDateTime dt(info.dateTime);
        entry.datatime = dt.toString(Qt::ISODate).toStdString();

        c.push_back(entry);
    }

    zip.close();
}

std::string QTContentZIPService::formatText() {
    if (c.empty()) return "";

    if (c.size() == 1 && (c[0].path == "Can not open file" || c[0].path == "Can not get info about file")) {
        return "";
    }

    std::ostringstream result;

    long long totalBytes = 0;
    size_t totalFiles = c.size();
    for (const auto& item : c) totalBytes += item.uncompressSize;
    double totalMB = static_cast<double>(totalBytes) / (1024 * 1024);

    result << "Total files: " << totalFiles
           << " | Total megabytes: " << std::fixed << std::setprecision(3) << totalMB << " MB\n";

    for (const auto& item : c) {
        double fileMB = static_cast<double>(item.uncompressSize) / (1024 * 1024);

        QDateTime dt = QDateTime::fromString(QString::fromStdString(item.datatime), Qt::ISODate);
        QString dateStr = dt.isValid() ? dt.toString("dd-MM-yyyy") : QString::fromStdString(item.datatime);

        result << item.path << " | "
               << std::fixed << std::setprecision(3) << fileMB << " MB | "
               << dateStr.toStdString() << "\n";
    }

    return result.str();
}