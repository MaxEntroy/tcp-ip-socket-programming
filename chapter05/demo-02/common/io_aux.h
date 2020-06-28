#ifndef IO_AUX_H_
#define IO_AUX_H_

#define BUF_SZ 128


// Read n bytes from fd
void io_read_n(int fd, char buf[], int n);

#endif
