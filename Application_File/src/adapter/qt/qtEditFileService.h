#pragma once

#include "../../core/editFileService.h"

#include <QFile>
#include <QLockFile>
#include <QString>
#include <QIODevice>
#include <QTextStream>

#include <stdexcept>

class QTEditFileService : public EditFile {
    private:
    QFile file;
    QLockFile lock;
    public:
    explicit QTEditFileService (const QString& path) : file(path), lock(path+".lock") {
        if (!lock.tryLock()) {
            throw std::runtime_error("file is already locked");
        }

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            lock.unlock();
            throw std::runtime_error("cannot open file");
        }
    }
    ~QTEditFileService() override {
        file.close();
        lock.unlock();
    }
    std::string getText() override;
    void setText(std::string text) override;
};