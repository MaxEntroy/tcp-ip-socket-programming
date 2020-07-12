#include "io_aux.h"

#include <errno.h>
#include <unistd.h>

namespace utils {

void io_read_n(int fd, char buf[], int n) {
  int total = 0;
  while(total < n) {
    int nread = read(fd, buf+total, n - total);
    if (nread == -1) {
      if (errno == EINTR)
        nread = 0;
      else
        return;
    }
    else if (nread == 0)
      break;

    total += nread;
  }
}

} // namespace utils
