#include "linuxDiskService.h"

void LinuxDiskService::setDisk () {
    if (!disk.empty()) disk.clear();

    std::ifstream mounts("/proc/mounts");
    if (!mounts.is_open()) return;

    std::string line;
    while (std::getline(mounts, line)) {
        std::istringstream iss(line);
        std::string device, mountPoint, fsType, options;
        int dump, pass;

        if (!(iss >> device >> mountPoint >> fsType >> options >> dump >> pass))
            continue;

        if (fsType == "proc" || fsType == "sysfs" || fsType == "tmpfs" ||
            fsType == "devtmpfs" || fsType == "devpts" || fsType == "cgroup")
            continue;

        struct statvfs stat;
        if (statvfs(mountPoint.c_str(), &stat) != 0)
            continue;

        DiskInfo info;
        info.nameD = device;
        info.fileSystem = fsType;
        info.memoryCapacity = stat.f_blocks * stat.f_frsize;
        info.memoryFree = stat.f_bfree * stat.f_frsize;
        info.memoryAviable = stat.f_bavail * stat.f_frsize;
        info.memoryOccupied = info.memoryCapacity - info.memoryFree;
        info.readOnly = (stat.f_flag & ST_RDONLY) != 0;

        disk.push_back(info);
    }
}

std::vector<DiskInfo> LinuxDiskService::getDisk() {
    return disk;
}

void LinuxDiskService::setInfoLine () {
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

std::string LinuxDiskService::getInfoLine () {
    return infoLine;
}

#include <iostream>

int main(int argc, char const *argv[])
{
    LinuxDiskService l;
    l.setDisk();
    l.setInfoLine();
    std::cout << l.getInfoLine();
    return 0;
}
