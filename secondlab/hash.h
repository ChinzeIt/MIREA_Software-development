#pragma once

#include <openssl/evp.h>

#include <atomic>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace AlgHash {

class Hash {
 private:
  std::vector<std::string> hash_;
  size_t countThread_;

  static std::string compute_md5(const std::string& s);
  static std::string compute_sha256(const std::string& s);

  static void index_to_candidate(size_t idx, std::string& out);

 public:
  std::map<unsigned int, std::string> password;
  std::map<unsigned int, double> time;

  Hash(const std::vector<std::string>& hashes, size_t countThread = 1)
      : hash_(hashes), countThread_(countThread) {}

  ~Hash() = default;

  void AlgorithmHash();
};

}  // namespace AlgHash
