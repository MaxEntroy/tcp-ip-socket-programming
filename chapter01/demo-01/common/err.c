#include "err.h"

#include <stdlib.h>
#include <errno.h>

void s_message(const char* pname, const char* msg) {
  if(pname) writestringerror("%s:", pname);
  writestringerror("%s\n", msg);
}

void v_message(const char* pname, int val) {
  if(pname) writestringerror("%s:", pname);
  writestringerror("%d\n", val);
}

void err_handling(const char* pname, const char* msg) {
  s_message(pname, msg);
  exit(EXIT_FAILURE);
}

void perr_handling(const char* pname, const char* msg) {
  s_message(pname, msg);
  v_message("errno", errno);
  exit(EXIT_FAILURE);
}
