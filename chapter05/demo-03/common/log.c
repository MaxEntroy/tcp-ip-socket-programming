#include "log.h"

#include <stdio.h>
#include <stdlib.h>

pzlog LOG_INIT(const char* conf, const char* cat) {
  int ret = zlog_init(conf);
  if(ret) {
    fprintf(stderr, "%s", "zlog init failed");
    exit(EXIT_FAILURE);
  }

  pzlog c = zlog_get_category(cat);
  if(!c) {
    fprintf(stderr, "%s", "zlog get cat failed");
    zlog_fini();
    exit(EXIT_FAILURE);
  }

  return c;
}

void LOG_FINI() {
  zlog_fini();
}
