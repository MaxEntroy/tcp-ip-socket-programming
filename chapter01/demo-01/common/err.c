#include "err.h"

void l_message(const char* pname, const char* msg) {
  if(pname) writestringerror("%s:", pname);
  writestringerror("%s\n", msg);
}
