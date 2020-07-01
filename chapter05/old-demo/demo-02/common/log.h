#ifndef LOG_H_
#define LOG_H_

#include <zlog/zlog.h>

typedef int LogLevel;

/* log level*/
#define DEBUG   0
#define INFO    1
#define NOTICE  2
#define WARN    3
#define ERROR   4
#define FATAL   5

/* handler of zlog */
typedef zlog_category_t *pzlog;

/* zlog manipulattion*/
pzlog LOG_INIT(const char* conf, const char* cat);

#if !defined(LOG)
#define LOG(c, level, ...)                                     \
  switch(level) {                                              \
    case DEBUG  : zlog_debug(c, __VA_ARGS__);break;            \
    case INFO   : zlog_info(c, __VA_ARGS__); break;            \
    case NOTICE : zlog_notice(c, __VA_ARGS__); break;          \
    case WARN   : zlog_warn(c, __VA_ARGS__); break;            \
    case ERROR  : zlog_error(c, __VA_ARGS__); break;           \
    case FATAL  : zlog_fatal(c, __VA_ARGS__); break;           \
    default     : zlog_error(c, "Undefined log level."); break;\
}
#endif

void LOG_FINI();

#endif
