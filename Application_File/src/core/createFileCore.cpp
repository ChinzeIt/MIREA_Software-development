#include "createFileService.h"

#include <fstream>
#include <filesystem>

class StdCreateFileService : public createFileService {
    public:
    bool createFile(const std::string& rawPath) override {
        std::filesystem::path filePath(rawPath);

        std::ofstream outFile(rawPath);
        if (!outFile) {
            outFile.close();
            return false;
        }

        outFile.close();

        return true;
    }
};
