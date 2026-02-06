#include "workFile.h"

namespace files {
// PRIVATE
// PUBLIC
bool filesJSON::createJson() {
  if (fsystem::exists(filePath_)) return false;

  std::ofstream ofs(filePath_);
  if (!ofs) return false;
  ofs << "{}";
  return true;
}

bool filesJSON::readJson(std::map<std::string, std::string>& outData) {
  if (!fsystem::exists(filePath_)) return false;

  std::ifstream ifs(filePath_);
  if (!ifs) return false;

  nlohmann::json j;
  try {
    ifs >> j;
  } catch (const nlohmann::json::parse_error&) {
    return false;
  }

  if (j.empty()) return false;

  outData.clear();
  for (auto it = j.begin(); it != j.end(); ++it) {
    if (it.value().is_string()) {
      outData[it.key()] = it.value();
    } else {
      outData[it.key()] = it.value().dump();
    }
  }

  return true;
}

bool filesJSON::deleteJson() {
  if (fsystem::exists(filePath_)) return fsystem::remove(filePath_);
  return false;
}
}  // namespace files