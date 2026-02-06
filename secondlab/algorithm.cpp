#include "hash.h"

namespace AlgHash {

void Hash::index_to_candidate(size_t idx, std::string& out) {
  const char base = 'a';
  out.resize(5);
  for (int pos = 4; pos >= 0; --pos) {
    out[pos] = static_cast<char>(base + (idx % 26));
    idx /= 26;
  }
}

std::string Hash::compute_md5(const std::string& s) {
  unsigned char result[EVP_MAX_MD_SIZE];
  unsigned int result_len = 0;
  EVP_MD_CTX* ctx = EVP_MD_CTX_new();
  if (!ctx) return {};
  if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1 ||
      EVP_DigestUpdate(ctx, s.data(), s.size()) != 1 ||
      EVP_DigestFinal_ex(ctx, result, &result_len) != 1) {
    EVP_MD_CTX_free(ctx);
    return {};
  }
  EVP_MD_CTX_free(ctx);
  std::ostringstream ss;
  ss << std::hex << std::setfill('0');
  for (unsigned int i = 0; i < result_len; ++i)
    ss << std::setw(2) << (static_cast<int>(result[i]) & 0xff);
  return ss.str();
}

std::string Hash::compute_sha256(const std::string& s) {
  unsigned char result[EVP_MAX_MD_SIZE];
  unsigned int result_len = 0;
  EVP_MD_CTX* ctx = EVP_MD_CTX_new();
  if (!ctx) return {};
  if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1 ||
      EVP_DigestUpdate(ctx, s.data(), s.size()) != 1 ||
      EVP_DigestFinal_ex(ctx, result, &result_len) != 1) {
    EVP_MD_CTX_free(ctx);
    return {};
  }
  EVP_MD_CTX_free(ctx);
  std::ostringstream ss;
  ss << std::hex << std::setfill('0');
  for (unsigned int i = 0; i < result_len; ++i)
    ss << std::setw(2) << (static_cast<int>(result[i]) & 0xff);
  return ss.str();
}

void Hash::AlgorithmHash() {
  using clk = std::chrono::steady_clock;

  const size_t n = hash_.size();
  if (n == 0) return;

  std::vector<bool> is_md5(n, false), is_sha(n, false);
  size_t md5_targets = 0, sha_targets = 0;
  for (size_t i = 0; i < n; ++i) {
    if (hash_[i].size() == 32) {
      is_md5[i] = true;
      ++md5_targets;
    } else if (hash_[i].size() == 64) {
      is_sha[i] = true;
      ++sha_targets;
    }
  }
  if (md5_targets == 0 && sha_targets == 0) return;

  std::vector<std::atomic<bool>> found(n);
  for (auto& f : found) f = false;
  std::atomic<size_t> found_md5(0), found_sha(0);
  std::atomic<bool> done(false);

  unsigned int base_key = 0;
  if (!time.empty()) base_key = time.rbegin()->first;
  std::atomic<unsigned int> next_key(base_key + 1);

  std::mutex write_mutex;

  auto t_start = clk::now();
  std::chrono::steady_clock::time_point last_mark = t_start;

  auto record_match = [&](const std::string& cand) {
    std::lock_guard<std::mutex> lk(write_mutex);
    auto now = clk::now();
    double seconds =
        std::chrono::duration_cast<std::chrono::microseconds>(now - last_mark)
            .count() /
        1e6;
    double rounded = std::round(seconds * 1000.0) / 1000.0;
    unsigned int key = next_key.fetch_add(1);
    password[key] = cand;
    time[key] = rounded;
    last_mark = now;
  };

  const size_t TOTAL = 11881376;

  auto worker = [&](size_t start_idx, size_t end_idx) {
    std::string cand;
    for (size_t idx = start_idx; idx < end_idx && !done.load(); ++idx) {
      index_to_candidate(idx, cand);

      std::string hmd5, hsha;
      if (found_md5.load() < md5_targets) hmd5 = compute_md5(cand);
      if (found_sha.load() < sha_targets) hsha = compute_sha256(cand);

      for (size_t i = 0; i < n; ++i) {
        if (found[i].load()) continue;
        if (is_md5[i] && !hmd5.empty() && hmd5 == hash_[i]) {
          bool expected = false;
          if (found[i].compare_exchange_strong(expected, true)) {
            found_md5.fetch_add(1);
            record_match(cand);
          }
        } else if (is_sha[i] && !hsha.empty() && hsha == hash_[i]) {
          bool expected = false;
          if (found[i].compare_exchange_strong(expected, true)) {
            found_sha.fetch_add(1);
            record_match(cand);
          }
        }
      }

      if (found_md5.load() == md5_targets && found_sha.load() == sha_targets) {
        done.store(true);
        break;
      }
    }
  };

  size_t threads = countThread_ <= 1 ? 1 : countThread_;
  if (threads == 1) {
    worker(0, TOTAL);
  } else {
    std::vector<std::thread> thr;
    thr.reserve(threads);
    size_t chunk = TOTAL / threads;
    size_t rem = TOTAL % threads;
    size_t cur = 0;
    for (size_t t = 0; t < threads; ++t) {
      size_t start = cur;
      size_t len = chunk + (t < rem ? 1 : 0);
      size_t end = start + len;
      thr.emplace_back(worker, start, end);
      cur = end;
    }
    for (auto& th : thr)
      if (th.joinable()) th.join();
  }

  return;
}

}  // namespace AlgHash
