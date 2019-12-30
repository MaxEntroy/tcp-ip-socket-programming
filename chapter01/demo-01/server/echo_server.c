#include "common/err.h"

int main(int argc, char* argv[]) {
  if(argc > 0) l_message(argv[0], "hello, world!");
  return 0;
}
