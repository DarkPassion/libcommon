
#ifndef _STRING_STRING_PRINTF_H_
#define _STRING_STRING_PRINTF_H_

#include <stdarg.h>
#include <string>
#include <vector>


namespace libcommon {
    
    // Return a C++ string
    extern std::string StringPrintf(const char* format, ...);
    
    // Store result into a supplied string and return it
    extern const std::string& SStringPrintf(std::string* dst, const char* format, ...);
    
    // Append result to a supplied string
    extern void StringAppendF(std::string* dst, const char* format, ...);
    
    // Lower-level routine that takes a va_list and appends to a specified
    // string.  All other routines are just convenience wrappers around it.
    extern void StringAppendV(std::string* dst, const char* format, va_list ap);
    
    // The max arguments supported by StringPrintfVector
    extern const int kStringPrintfVectorMaxArgs;
    
    // You can use this version when all your arguments are strings, but
    // you don't know how many arguments you'll have at compile time.
    // StringPrintfVector will LOG(FATAL) if v.size() > kStringPrintfVectorMaxArgs
    extern std::string StringPrintfVector(const char* format, const std::vector<std::string>& v);

}


#endif


