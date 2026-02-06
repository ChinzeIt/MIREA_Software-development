#include "workFile.h"

namespace files {
// PRIVATE
// PUBLIC
bool filesTXT::createFile() {
  if (fsystem::exists(filePath_)) return false;

  std::ofstream ofs(filePath_);
  if (!ofs) return false;

  return true;
}

bool filesTXT::writeLine(const std::string& line) {
  if (!fsystem::exists(filePath_)) return false;

  std::ofstream ofs(filePath_, std::ios::app);
  if (!ofs) return false;
  ofs << line << "\n";
  return true;
}

bool filesTXT::readFile(std::vector<std::string>& outLines) {
  outLines.clear();

  if (!fsystem::exists(filePath_)) return false;

  std::ifstream ifs(filePath_);
  if (!ifs) return false;

  std::string line;
  bool hasContent = false;
  while (std::getline(ifs, line)) {
    outLines.push_back(line);
    hasContent = true;
  }

  return hasContent;
}

bool filesTXT::deleteFile() {
  if (std::filesystem::exists(filePath_))
    return std::filesystem::remove(filePath_);
  return false;
}
}  // namespace files