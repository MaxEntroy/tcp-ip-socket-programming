#ifndef APP_BUFFER_H_
#define APP_BUFFER_H_

#include <vector>

namespace utils {

const int kDefaultBufLen = 1024;

struct Buffer {
  Buffer() { data_.resize(kDefaultBufLen); Clear(); }

  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  void Clear() { ridx_ = widx_ = 0; }

  bool IsReadable(int n) const { return widx_ - ridx_ == n; }

  std::vector<char> data_;
  int ridx_;
  int widx_;
};

}  // namespace utils

#endif  // APP_BUFFER_H_
