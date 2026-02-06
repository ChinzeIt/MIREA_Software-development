#include "workFile.h"

namespace files {
// PRIVATE
bool infoDisk::getInfoDisk() {
  disk_.clear();

  fstream mounts("/proc/mounts");
  if (!mounts.is_open()) {
    return false;
  }

  std::map<string, string> infoMap;
  try {
    for (auto &entry : fsystem::directory_iterator("/dev/disk/by-partlabel")) {
      std::error_code ec;
      auto target = fsystem::read_symlink(entry.path(), ec);
      if (!ec) {
        string dev = "/dev/" + target.filename().string();
        string label = entry.path().filename().string();
        infoMap[dev] = label;
      }
    }
  } catch (const fsystem::filesystem_error &) {
  }

  string line;
  while (std::getline(mounts, line)) {
    std::istringstream iss(line);
    string dev, mountpoint, fstype, opts;
    if (!(iss >> dev >> mountpoint >> fstype >> opts)) continue;

    unsigned long long sizeBytes = 0;
    string devName = fsystem::path(dev).filename();
    string sysPath = "/sys/class/block/" + devName + "/size";
    std::ifstream sizeFile(sysPath);
    if (sizeFile.is_open()) {
      unsigned long long sectors = 0;
      sizeFile >> sectors;
      sizeBytes = sectors * 512ULL;
    } else {
      sizeBytes = 0;
    }

    DiskInfo_ d;
    d.device_ = dev;
    d.mountpoint_ = mountpoint;
    d.fstype_ = fstype;
    d.sizeBytes_ = sizeBytes;
    d.label_ = infoMap.count(dev) ? infoMap[dev] : "";

    disk_.push_back(d);
  }

  return true;
}
// PUBLIC
void infoDisk::outInfoDisk() {
  infoDisk out;
  if (!out.getInfoDisk()) {
    std::cout << "Problem in get information about disk\n";
    return;
  }
  std::cout << "Information about disk\n";
  std::cout << "|---------------------|\n";
  for (const auto &d : out.disk_) {
    std::cout << "Device: " << d.device_ << "\n";
    std::cout << "Label : " << (d.label_.empty() ? "Label not found" : d.label_)
              << "\n";
    std::cout << "Mount : " << d.mountpoint_ << "\n";
    std::cout << "FS    : " << d.fstype_ << "\n";
    std::cout << "Size  : " << d.sizeBytes_ / (1024 * 1024) << " MB\n";
    std::cout << "|---------------------|\n";
  }
}

bool infoDisk::checkedPath(fsystem::path &filePath_) {
  std::string pathStr = filePath_.string();
  if (!pathStr.empty() && pathStr[0] == '~') {
    const char *home = std::getenv("HOME");
    if (home) {
      pathStr.replace(0, 1, home);
    }
  }

  std::error_code ec;
  fsystem::path absPath = fsystem::weakly_canonical(pathStr, ec);
  if (ec) {
    return false;
  }

  fsystem::path cwd = fsystem::weakly_canonical(fsystem::current_path(), ec);
  if (ec) {
    return false;
  }

  auto absStr = absPath.string();
  auto cwdStr = cwd.string();

  if (absStr.rfind(cwdStr + "/", 0) != 0 && absStr != cwdStr) {
    return false;
  }

  fsystem::path dir = absPath.parent_path();
  if (!dir.empty() && !fsystem::exists(dir)) {
    return false;
  }

  filePath_ = absPath;

  return true;
}
}  // namespace files