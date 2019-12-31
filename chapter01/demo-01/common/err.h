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
void s_message(const char* pname, const char* msg);

/* Prints an error value, add the program name in front of it
 * (if presents)
 * */
void v_message(const char* pname, int val);

/* Error handling.
 * */
void err_handling(const char* pname, const char* msg);

/* Error handling.
 * Prints errno.
 * */
void perr_handling(const char* pname, const char* msg);

#endif
