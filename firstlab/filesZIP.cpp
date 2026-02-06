#include "workFile.h"

namespace files {
// PRIVATE
// PUBLIC
bool filesZIP::createZIP() {
  if (fsystem::exists(archivePath_)) return false;

  zipFile zf = zipOpen(archivePath_.string().c_str(), APPEND_STATUS_CREATE);
  if (!zf) return false;

  if (zipClose(zf, nullptr) != ZIP_OK) return false;

  return true;
}

bool filesZIP::addFile(const string& filename) {
  if (!fsystem::exists(filename)) return false;

  fsystem::path filePath = filename;

  if (filePath.string().find('/') != std::string::npos ||
      filePath.string().find('\\') != std::string::npos) {
    bool status = infoDisk::checkedPath(filePath);
    if (!status) return false;
  }

  std::vector<std::string> existingFiles;
  readHaveInZip(existingFiles);
  std::string entryName = filePath.filename().generic_string();

  for (const auto& f : existingFiles) {
    if (f == entryName) {
      return false;
    }
  }

  zipFile zf = zipOpen(archivePath_.string().c_str(), APPEND_STATUS_ADDINZIP);
  if (!zf) return false;

  if (zipOpenNewFileInZip(zf, entryName.c_str(), nullptr, nullptr, 0, nullptr,
                          0, nullptr, Z_DEFLATED,
                          Z_DEFAULT_COMPRESSION) != ZIP_OK) {
    zipClose(zf, nullptr);
    return false;
  }

  std::ifstream ifs(filename, std::ios::binary);
  if (!ifs) {
    zipCloseFileInZip(zf);
    zipClose(zf, nullptr);
    return false;
  }

  const size_t CHUNK = 16384;
  std::vector<char> buf(CHUNK);
  while (ifs) {
    ifs.read(buf.data(), buf.size());
    std::streamsize r = ifs.gcount();
    if (r > 0) {
      int writeErr =
          zipWriteInFileInZip(zf, buf.data(), static_cast<unsigned int>(r));
      if (writeErr != ZIP_OK) {
        zipCloseFileInZip(zf);
        zipClose(zf, nullptr);
        return false;
      }
    }
  }

  if (zipCloseFileInZip(zf) != ZIP_OK) {
    zipClose(zf, nullptr);
    return false;
  }

  if (zipClose(zf, nullptr) != ZIP_OK) return false;

  return true;
}

bool filesZIP::readHaveInZip(std::vector<std::string>& fileList) {
  fileList.clear();

  if (!fsystem::exists(archivePath_)) return false;

  unzFile zipFile = unzOpen(archivePath_.string().c_str());
  if (!zipFile) return false;

  if (unzGoToFirstFile(zipFile) != UNZ_OK) {
    unzClose(zipFile);
    return false;
  }

  do {
    char filenameInZip[256];
    unz_file_info fileInfo;
    if (unzGetCurrentFileInfo(zipFile, &fileInfo, filenameInZip,
                              sizeof(filenameInZip), nullptr, 0, nullptr,
                              0) != UNZ_OK) {
      unzClose(zipFile);
      return false;
    }
    fileList.push_back(filenameInZip);
  } while (unzGoToNextFile(zipFile) == UNZ_OK);

  unzClose(zipFile);

  return !fileList.empty();
}

bool filesZIP::readFileInZIP(const string& filenameInArchive,
                             std::vector<std::string>& lines) {
  lines.clear();

  if (!fsystem::exists(archivePath_)) return false;

  unzFile uf = unzOpen(archivePath_.string().c_str());
  if (!uf) return false;

  if (unzLocateFile(uf, filenameInArchive.c_str(), 0) != UNZ_OK) {
    unzClose(uf);
    return false;
  }

  if (unzOpenCurrentFile(uf) != UNZ_OK) {
    unzClose(uf);
    return false;
  }

  const size_t CHUNK = 16384;
  std::vector<char> buffer(CHUNK);
  int bytesRead = 0;
  std::string currentLine;

  while ((bytesRead = unzReadCurrentFile(uf, buffer.data(), CHUNK)) > 0) {
    for (int i = 0; i < bytesRead; ++i) {
      if (buffer[i] == '\n') {
        lines.push_back(currentLine);
        currentLine.clear();
      } else {
        currentLine += buffer[i];
      }
    }
  }
  if (!currentLine.empty()) lines.push_back(currentLine);

  unzCloseCurrentFile(uf);
  unzClose(uf);

  return true;
}

bool filesZIP::extractFileFromZip(const string& filenameInArchive) {
  if (!fsystem::exists(archivePath_)) return false;

  unzFile uf = unzOpen(archivePath_.string().c_str());
  if (!uf) return false;

  if (unzLocateFile(uf, filenameInArchive.c_str(), 0) != UNZ_OK) {
    unzClose(uf);
    return false;
  }

  unz_file_info fileInfo;
  if (unzGetCurrentFileInfo(uf, &fileInfo, nullptr, 0, nullptr, 0, nullptr,
                            0) != UNZ_OK) {
    unzClose(uf);
    return false;
  }

  auto spaceAvailable = fsystem::space(fsystem::current_path()).available;
  if (spaceAvailable < fileInfo.uncompressed_size) {
    unzClose(uf);
    return false;
  }

  if (unzOpenCurrentFile(uf) != UNZ_OK) {
    unzClose(uf);
    return false;
  }

  std::ofstream ofs(filenameInArchive, std::ios::binary);
  if (!ofs) {
    unzCloseCurrentFile(uf);
    unzClose(uf);
    return false;
  }

  const size_t CHUNK = 16384;
  std::vector<char> buffer(CHUNK);
  int bytesRead = 0;
  while ((bytesRead = unzReadCurrentFile(uf, buffer.data(), CHUNK)) > 0) {
    ofs.write(buffer.data(), bytesRead);
  }

  ofs.close();
  unzCloseCurrentFile(uf);
  unzClose(uf);

  return deleteFileInZip(filenameInArchive);
}

bool filesZIP::deleteFileInZip(const string& filenameInArchive) {
  if (!fsystem::exists(archivePath_)) return false;

  std::vector<std::string> fileList;
  if (!readHaveInZip(fileList)) return false;

  auto it = std::find(fileList.begin(), fileList.end(), filenameInArchive);
  if (it == fileList.end()) {
    return false;
  }

  fsystem::path tempPath = archivePath_.string() + ".tmp";
  zipFile newZip = zipOpen(tempPath.string().c_str(), APPEND_STATUS_CREATE);
  if (!newZip) return false;

  unzFile uf = unzOpen(archivePath_.string().c_str());
  if (!uf) {
    zipClose(newZip, nullptr);
    return false;
  }

  if (unzGoToFirstFile(uf) == UNZ_OK) {
    do {
      char filename[256];
      if (unzGetCurrentFileInfo64(uf, nullptr, filename, sizeof(filename),
                                  nullptr, 0, nullptr, 0) != UNZ_OK) {
        continue;
      }

      std::string currentFile(filename);
      if (currentFile == filenameInArchive) {
        continue;
      }

      if (unzOpenCurrentFile(uf) != UNZ_OK) continue;

      if (zipOpenNewFileInZip(newZip, filename, nullptr, nullptr, 0, nullptr, 0,
                              nullptr, Z_DEFLATED,
                              Z_DEFAULT_COMPRESSION) != ZIP_OK) {
        unzCloseCurrentFile(uf);
        continue;
      }

      const size_t BUF_SIZE = 16384;
      std::vector<char> buffer(BUF_SIZE);
      int bytesRead;
      while ((bytesRead =
                  unzReadCurrentFile(uf, buffer.data(), buffer.size())) > 0) {
        zipWriteInFileInZip(newZip, buffer.data(), bytesRead);
      }

      unzCloseCurrentFile(uf);
      zipCloseFileInZip(newZip);

    } while (unzGoToNextFile(uf) == UNZ_OK);
  }

  unzClose(uf);
  zipClose(newZip, nullptr);

  fsystem::remove(archivePath_);
  fsystem::rename(tempPath, archivePath_);

  return true;
}

std::uintmax_t filesZIP::sizeZIP() const {
  if (!valid_) return 0;
  if (!fsystem::exists(archivePath_)) return 0;

  try {
    return fsystem::file_size(archivePath_);
  } catch (...) {
    return 0;
  }
}

bool filesZIP::deleteZIP() {
  if (fsystem::exists(archivePath_)) return fsystem::remove(archivePath_);
  return false;
}

}  // namespace files