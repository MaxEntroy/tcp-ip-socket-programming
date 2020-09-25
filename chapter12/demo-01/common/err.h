#ifndef UTIL_ERR_H_
#define UTIL_ERR_H_

namespace utils {

/* Error handling.
 * */
void err_handling(const char* pname, const char* msg);

/* Error handling.
 * Prints errno.
 * */
void perr_handling(const char* pname, const char* msg);

} // namespace utils

#endif
