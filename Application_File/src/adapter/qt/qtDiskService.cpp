#include "qtDiskService.h"

void QTDiskService::setDisk () {
    if (!disk.empty()) disk.clear();

    for (const auto& storage : QStorageInfo::mountedVolumes()) {
        if (!storage.isValid() || !storage.isReady()) continue;

        DiskInfo info;

        info.nameD = storage.displayName().toStdString();
        info.fileSystem = storage.fileSystemType();
        info.memoryCapacity = storage.bytesFree();
        info.memoryFree = storage.bytesFree();
        info.memoryAviable = storage.bytesAvailable();
        info.memoryOccupied = storage.bytesTotal() - storage.bytesFree();
        info.readOnly = storage.isReadOnly();

        disk.push_back(info);
    }
}

std::vector<DiskInfo> QTDiskService::getDisk () {
    return disk;
}

void QTDiskService::setInfoLine () {
    if (!infoLine.empty()) infoLine.clear();

    for (const DiskInfo& storage : disk) {
        infoLine.append("Name: " + storage.nameD + "\n");
        infoLine.append("Filesystem: " + storage.fileSystem + "\n");
        infoLine.append("Memort capacity: " + std::to_string(storage.memoryCapacity) + "\n");
        infoLine.append("Memory free: " + std::to_string(storage.memoryFree) + "\n");
        infoLine.append("Memory aviable: " + std::to_string(storage.memoryAviable) + "\n");
        infoLine.append("Memory occupied: " + std::to_string(storage.memoryOccupied) + "\n");
        infoLine.append("Is read only: " + std::string(storage.readOnly ? "yes" : "no") + "\n");
    }

    if (!infoLine.empty() && infoLine.back()=='\n') infoLine.pop_back();
}

std::string QTDiskService::getInfoLine () {
    return infoLine;
}