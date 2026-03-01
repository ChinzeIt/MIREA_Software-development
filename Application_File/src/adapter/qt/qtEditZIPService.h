#include "../../core/editZIPService.h"

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <quazip/JlCompress.h>
#include <QFile>
#include <QLockFile>
#include <QSet>

class QTEditZIPService : public EditZIPService {
    private:
    QFile file;
    QLockFile lock;
    public:
    std::function<void(int)> onProgress;

    explicit QTEditZIPService (const QString& path) : file(path), lock(path+".lock") {
        if (!lock.tryLock()) {
            throw std::runtime_error("file is already locked");
        }

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            lock.unlock();
            throw std::runtime_error("cannot open file");
        }
    }
    ~QTEditZIPService() override {
        file.close();
        lock.unlock();
    }
    void delFile(const std::vector<std::string>& vec) override;
    void comFile(std::string) override;
};