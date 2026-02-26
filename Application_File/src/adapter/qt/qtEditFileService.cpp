#include "qtEditFileService.h"

std::string QTEditFileService::getText () {
    if (!file.isOpen())
        return "Can not open file or content have damage";

    file.seek(0);

    QTextStream in(&file);

    return in.readAll().toStdString();
}

void QTEditFileService::setText (std::string text) {
    if (!file.isOpen())
        return;

    file.resize(0);
    file.seek(0);

    QTextStream out(&file);

    out << QString::fromStdString(text);
    out.flush();
}