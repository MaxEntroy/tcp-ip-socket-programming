#ifndef UTILS_IO_AUX_H_
#define UTILS_IO_AUX_H_

namespace utils {

// Read n bytes from fd(for TCP)
void io_read_n(int fd, char buf[], int n);

} // namespace utils

#endif // UTILS_IO_AUX_H_
