#pragma once

#include <minizip/unzip.h>
#include <minizip/zip.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "tinyxml2.h"

namespace fsystem = std::filesystem;

namespace files {

class infoDisk {
 private:
  using fstream = std::ifstream;
  using string = std::string;

  struct DiskInfo_ {
    string device_;
    string mountpoint_;
    string fstype_;
    string label_;
    unsigned long long sizeBytes_;
  };

  std::vector<DiskInfo_> disk_;

  bool getInfoDisk();

 public:
  static void outInfoDisk();
  static bool checkedPath(fsystem::path& filePath_);
};

class filesTXT {
 private:
  using string = std::string;
  string filename_;
  fsystem::path filePath_;
  bool valid_;

 public:
  explicit filesTXT(const string& filename)
      : filename_(filename), valid_(true) {
    filePath_ = filename_;

    fsystem::path ext = filePath_.extension();

    if (ext.empty()) {
      filePath_ += ".txt";
    } else if (ext != ".txt") {
      valid_ = false;
    }

    if (filePath_.string().find('/') != std::string::npos ||
        filePath_.string().find('\\') != std::string::npos) {
      valid_ = infoDisk::checkedPath(filePath_);
    }
  }

  bool isValid() const { return valid_; }

  bool createFile();

  bool writeLine(const string& line);

  bool readFile(std::vector<std::string>& outLines);

  bool deleteFile();
};

class filesJSON {
 private:
  using string = std::string;
  string filename_;
  fsystem::path filePath_;
  bool valid_;

 public:
  explicit filesJSON(const string& filename)
      : filename_(filename), valid_(true) {
    filePath_ = filename_;

    fsystem::path ext = filePath_.extension();

    if (ext.empty()) {
      filePath_ += ".json";
    } else if (ext != ".json") {
      valid_ = false;
    }

    if (filePath_.string().find('/') != std::string::npos ||
        filePath_.string().find('\\') != std::string::npos) {
      valid_ = infoDisk::checkedPath(filePath_);
    }
  }

  bool isValid() const { return valid_; }

  bool createJson();

  template <typename T>
  bool writeJson(const T& obj) {
    if (!fsystem::exists(filePath_)) return false;
    std::ofstream ofs(filePath_);
    if (!ofs) return false;

    nlohmann::json j = obj;
    ofs << j.dump(4);
    return true;
  }

  bool readJson(std::map<std::string, std::string>& outData);

  bool deleteJson();
};

class filesXML {
 private:
  using string = std::string;
  std::string filename_;
  fsystem::path filePath_;
  bool valid_;

 public:
  explicit filesXML(const string& filename)
      : filename_(filename), valid_(true) {
    filePath_ = filename_;

    fsystem::path ext = filePath_.extension();

    if (ext.empty()) {
      filePath_ += ".xml";
    } else if (ext != ".xml") {
      valid_ = false;
    }

    if (filePath_.string().find('/') != std::string::npos ||
        filePath_.string().find('\\') != std::string::npos) {
      valid_ = infoDisk::checkedPath(filePath_);
    }
  }

  bool isValid() const { return valid_; }

  bool createXML();

  bool addDataXML(const string& tagName, const string& value);
  bool addDataXML(const std::map<string, string>& data);

  bool readXML(std::map<std::string, std::string>& data);

  bool deleteXML();
};

class filesZIP {
 private:
  using string = std::string;
  using size = std::size_t;
  string archiveName_;
  fsystem::path archivePath_;
  bool valid_;

 public:
  explicit filesZIP(const string& name) : archiveName_(name), valid_(true) {
    archivePath_ = archiveName_;

    fsystem::path ext = archivePath_.extension();

    if (ext.empty()) {
      archivePath_ += ".zip";
    } else if (ext != ".zip") {
      valid_ = false;
    }

    if (archivePath_.string().find('/') != std::string::npos ||
        archivePath_.string().find('\\') != std::string::npos) {
      valid_ = infoDisk::checkedPath(archivePath_);
    }
  }

  bool isValid() const { return valid_; }

  bool createZIP();

  bool addFile(const string& filename);

  bool readHaveInZip(std::vector<std::string>& fileList);

  bool readFileInZIP(const string& filenameInArchive,
                     std::vector<std::string>& lines);

  bool extractFileFromZip(const string& filenameInArchive);

  bool deleteFileInZip(const string& filenameInArchive);

  std::uintmax_t sizeZIP() const;

  bool deleteZIP();
};

}  // namespace files