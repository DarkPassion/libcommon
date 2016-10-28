#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/stat.h>       // mkdir
#include <sstream>
#include <time.h>
#ifdef ANROID_DEV
#include <android/log.h>
#define ANDROID_LOG_TAG     "libcommon"
#endif
#include "log.h"

using namespace std;

std::string log::get_cur_time()
{
    std::string s;
    time_t tt;
    time(&tt);
    struct tm* timeinfo;
    timeinfo = localtime(&tt);

    char buff[256] = {0};
    sprintf(buff, "%d_%d_%d.log", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);

    s = buff;
    return s;
}

std::string log::get_cur_sec()
{
    std::string s;
    time_t tt;
    time(&tt);
    struct tm* timeinfo;
    timeinfo = localtime(&tt);

    char buff[256] = {0};

    timeinfo->tm_hour > 9 ? sprintf(buff, "%d", timeinfo->tm_hour) : sprintf(buff, "0%d", timeinfo->tm_hour);
    s += buff;
    s += ":";
    memset(buff, 0, sizeof(buff));
    timeinfo->tm_min > 9 ? sprintf(buff, "%d", timeinfo->tm_min) : sprintf(buff, "0%d", timeinfo->tm_min);

    s += buff;
    s += ":";
    memset(buff, 0, sizeof(buff));
    timeinfo->tm_sec > 9 ? sprintf(buff, "%d", timeinfo->tm_sec) : sprintf(buff, "0%d", timeinfo->tm_sec);
    s += buff;
    return s;
}

void log::write_log(int severity, const char* msg)
{
    char severity_str[255] = {0};

    switch (severity)
    {
    case _CR_LOG_DEBUG:
        memcpy(severity_str, "Debug", strlen("Debug"));
        break;
    case _CR_LOG_INFO:
        memcpy(severity_str, "Info", strlen("Info"));
        break;
    case _CR_LOG_WARN:
        memcpy(severity_str, "Warn", strlen("Warn"));
        break;
    case _CR_LOG_ERROR:
        memcpy(severity_str, "Error", strlen("Error"));
        break;
    }
    
#if OUTPUT_LOG_FILE
    mkdir("Log", 0755);
    string dir = "Log/" + get_cur_time();
    const char *file_name = dir.data();
    ofstream fs;
    fs.open(file_name, ios::app);
    fs << "[" << severity_str << "]"<< "[" << get_cur_sec().c_str() << "] " << msg << endl;
    fs.close();
#endif
    
#ifdef ANDROID_DEV
    if (severity <_CR_LOG_WARN) {
        __android_log_printf(ANDROID_LOG_DEBUG, ANDROID_LOG_TAG, __VA_ARGS__);
    } else {
        __android_log_printf(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, __VA_ARGS__);
    }
#endif
}

void log::cr_log(int severity, const char *fmt, va_list ap)
{
    char buf[1024];
    if(fmt != NULL)
    {
        int buf_len = sizeof(buf);
        vsnprintf(buf, buf_len, fmt, ap);
        buf[buf_len-1] = '\0';
    }
    else
    {
        buf[0] = '\0';
    }
    write_log(severity, buf);
}

void log::cr_debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_DEBUG, fmt, ap);

    va_end(ap);
}

void log::cr_info(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_INFO, fmt, ap);

    va_end(ap);
}

void log::cr_warn(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_WARN, fmt, ap);

    va_end(ap);
}

void log::cr_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    cr_log(_CR_LOG_ERROR, fmt, ap);

    va_end(ap);
}
