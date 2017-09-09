
#ifndef _TIMER_TIME_FORMAT_H_
#define _TIMER_TIME_FORMAT_H_

#include <string>
#include <stdint.h>

namespace libcommon {
    
    
    struct DateTime {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
    };
    
    // Converts a timestamp (seconds elapsed since 1970-01-01T00:00:00, could be
    // negative to represent time before 1970-01-01) to DateTime. Returns false
    // if the timestamp is not in the range between 0001-01-01T00:00:00 and
    // 9999-12-31T23:59:59.
    bool SecondsToDateTime(int64_t seconds, DateTime* time);
    // Converts DateTime to a timestamp (seconds since 1970-01-01T00:00:00).
    // Returns false if the DateTime is not valid or is not in the valid range.
    bool DateTimeToSeconds(const DateTime& time, int64_t* seconds);
    
    void GetCurrentTime(int64_t* seconds, int32_t* nanos);
    
    // Formats a time string in RFC3339 fromat.
    //
    // For example, "2015-05-20T13:29:35.120Z". For nanos, 0, 3, 6 or 9 fractional
    // digits will be used depending on how many are required to represent the exact
    // value.
    //
    // Note that "nanos" must in the range of [0, 999999999].
    std::string FormatTime(int64_t seconds, int32_t nanos);
    // Parses a time string. This method accepts RFC3339 date/time string with UTC
    // offset. For example, "2015-05-20T13:29:35.120-08:00".
    bool ParseTime(const std::string& value, int64_t* seconds, int32_t* nanos);

}


#endif
