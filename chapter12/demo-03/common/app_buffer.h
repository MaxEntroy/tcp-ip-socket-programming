#ifndef APP_BUFFER_H_
#define APP_BUFFER_H_

namespace utils {

class Buffer {
 public:
  Buffer() = default;
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;
 private:
};

}  // namespace utils

#endif  // APP_BUFFER_H_
