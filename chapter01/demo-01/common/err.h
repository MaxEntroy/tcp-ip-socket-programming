#ifndef ERR_H_
#define ERR_H_

/* Error handling.
 * */
void err_handling(const char* pname, const char* msg);

/* Error handling.
 * Prints errno.
 * */
void perr_handling(const char* pname, const char* msg);

#endif
