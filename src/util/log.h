#ifndef _UTIL_LOG_H_
#define _UTIL_LOG_H_

#include <string>
#include <stdarg.h>

#define _CR_LOG_DEBUG 0
#define _CR_LOG_INFO  1
#define _CR_LOG_WARN  2
#define _CR_LOG_ERROR 3

//using namespace std;

#define LOGD    log::cr_debug

#define LOGI    log::cr_info

#define LOGW    log::cr_warn

#define LOGE    log::cr_error


class log
{
public:
    static void write_log(int severity, const char* msg);
    static void cr_log(int severity, const char *fmt, va_list ap);
    static void cr_debug(const char *fmt, ...);
    static void cr_info(const char *fmt, ...);
    static void cr_warn(const char *fmt, ...);
    static void cr_error(const char *fmt, ...);


    static std::string get_cur_sec();
    static std::string get_cur_time();
};

#endif
