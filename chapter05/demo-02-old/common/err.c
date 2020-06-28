#include "err.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* print an error message. */
#if !defined(writestringerror)
#define writestringerror(s, p) \
        (fprintf(stderr, (s), (p)), fflush(stderr))
#endif

/* Prints an error message, adding the program name in front of it
 * (if presents)
 * */
static void s_message(const char* pname, const char* msg) {
  if(pname) writestringerror("%s:", pname);
  writestringerror("%s\n", msg);
}

/* Prints an error value, add the program name in front of it
 * (if presents)
 * */
static void v_message(const char* pname, int val) {
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
