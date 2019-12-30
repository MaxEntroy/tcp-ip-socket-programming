#ifndef ERR_H_
#define ERR_H_

#include <stdio.h>

/* print an error message. */
#if !defined(writestringerror)
#define writestringerror(s, p) \
        (fprintf(stderr, (s), (p)), fflush(stderr))
#endif

/* Prints an error message, adding the program name in front of it
 * (if presents)
 * */
void l_message(const char* pname, const char* msg);

#endif
