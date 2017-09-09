/*
 *  Copyright 2004 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef STRING_STRINGUTILS_H__
#define STRING_STRINGUTILS_H__

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#if defined(WEBRTC_WIN)
#include <malloc.h>
#include <wchar.h>
#define alloca _alloca
#endif  // WEBRTC_WIN

#if defined(WEBRTC_POSIX)
#ifdef BSD
#include <stdlib.h>
#else  // BSD
#include <alloca.h>
#endif  // !BSD
#endif  // WEBRTC_POSIX

#include <string>

#include "util/basictypes.h"

///////////////////////////////////////////////////////////////////////////////
// Generic string/memory utilities
///////////////////////////////////////////////////////////////////////////////

#define STACK_ARRAY(TYPE, LEN) static_cast<TYPE*>(::alloca((LEN)*sizeof(TYPE)))

namespace libcommon
{

    
    // Complement to memset.  Verifies memory consists of count bytes of value c.
    bool memory_check(const void* memory, int c, size_t count);
    
    // Determines whether the simple wildcard pattern matches target.
    // Alpha characters in pattern match case-insensitively.
    // Asterisks in pattern match 0 or more characters.
    // Ex: string_match("www.TEST.GOOGLE.COM", "www.*.com") -> true
    bool string_match(const char* target, const char* pattern);
    
    
    
    
    // ----------------------------------------------------------------------
    // ascii_isalnum()
    //    Check if an ASCII character is alphanumeric.  We can't use ctype's
    //    isalnum() because it is affected by locale.  This function is applied
    //    to identifiers in the protocol buffer language, not to natural-language
    //    strings, so locale should not be taken into account.
    // ascii_isdigit()
    //    Like above, but only accepts digits.
    // ascii_isspace()
    //    Check if the character is a space character.
    // ----------------------------------------------------------------------
    
    inline bool ascii_isalnum(char c) {
        return ('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z') ||
        ('0' <= c && c <= '9');
    }
    
    inline bool ascii_isdigit(char c) {
        return ('0' <= c && c <= '9');
    }
    
    inline bool ascii_isspace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
        c == '\r';
    }
    
    inline bool ascii_isupper(char c) {
        return c >= 'A' && c <= 'Z';
    }
    
    inline bool ascii_islower(char c) {
        return c >= 'a' && c <= 'z';
    }
    
    inline char ascii_toupper(char c) {
        return ascii_islower(c) ? c - ('a' - 'A') : c;
    }
    
    inline char ascii_tolower(char c) {
        return ascii_isupper(c) ? c + ('a' - 'A') : c;
    }
    
    inline int hex_digit_to_int(char c) {
        /* Assume ASCII. */
        int x = static_cast<unsigned char>(c);
        if (x > '9') {
            x += 9;
        }
        return x & 0xf;
    }
    
    
    
    // ----------------------------------------------------------------------
    // HasPrefixString()
    //    Check if a string begins with a given prefix.
    // StripPrefixString()
    //    Given a string and a putative prefix, returns the string minus the
    //    prefix string if the prefix matches, otherwise the original
    //    string.
    // ----------------------------------------------------------------------
    inline bool HasPrefixString(const std::string& str,
                                const std::string& prefix) {
        return str.size() >= prefix.size() &&
        str.compare(0, prefix.size(), prefix) == 0;
    }
    
    inline std::string StripPrefixString(const std::string& str, const std::string& prefix) {
        if (HasPrefixString(str, prefix)) {
            return str.substr(prefix.size());
        } else {
            return str;
        }
    }
    
    // ----------------------------------------------------------------------
    // HasSuffixString()
    //    Return true if str ends in suffix.
    // StripSuffixString()
    //    Given a string and a putative suffix, returns the string minus the
    //    suffix string if the suffix matches, otherwise the original
    //    string.
    // ----------------------------------------------------------------------
    inline bool HasSuffixString(const std::string& str,
                                const std::string& suffix) {
        return str.size() >= suffix.size() &&
        str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
    
    inline std::string StripSuffixString(const std::string& str, const std::string& suffix) {
        if (HasSuffixString(str, suffix)) {
            return str.substr(0, str.size() - suffix.size());
        } else {
            return str;
        }
    }

    // ----------------------------------------------------------------------
    // ReplaceCharacters
    //    Replaces any occurrence of the character 'remove' (or the characters
    //    in 'remove') with the character 'replacewith'.
    //    Good for keeping html characters or protocol characters (\t) out
    //    of places where they might cause a problem.
    // StripWhitespace
    //    Removes whitespaces from both ends of the given string.
    // ----------------------------------------------------------------------
    void ReplaceCharacters(std::string* s, const char* remove,
                                              char replacewith);
    void StripString(std::string* s, const char* remove,
                                        char replacewith);
    
    void StripWhitespace(std::string* s);

    
    // ----------------------------------------------------------------------
    // LowerString()
    // UpperString()
    // ToUpper()
    //    Convert the characters in "s" to lowercase or uppercase.  ASCII-only:
    //    these functions intentionally ignore locale because they are applied to
    //    identifiers used in the Protocol Buffer language, not to natural-language
    //    strings.
    // ----------------------------------------------------------------------
    
    inline void LowerString(std::string * s) {
        std::string::iterator end = s->end();
        for (std::string::iterator i = s->begin(); i != end; ++i) {
            // tolower() changes based on locale.  We don't want this!
            if ('A' <= *i && *i <= 'Z') *i += 'a' - 'A';
        }
    }
    
    inline void UpperString(std::string * s) {
        std::string::iterator end = s->end();
        for (std::string::iterator i = s->begin(); i != end; ++i) {
            // toupper() changes based on locale.  We don't want this!
            if ('a' <= *i && *i <= 'z') *i += 'A' - 'a';
        }
    }
    
    inline std::string ToUpper(const std::string& s) {
        std::string out = s;
        UpperString(&out);
        return out;
    }
    
    // ----------------------------------------------------------------------
    // StringReplace()
    //    Give me a string and two patterns "old" and "new", and I replace
    //    the first instance of "old" in the string with "new", if it
    //    exists.  RETURN a new string, regardless of whether the replacement
    //    happened or not.
    // ----------------------------------------------------------------------
    
    std::string StringReplace(const std::string& s, const std::string& oldsub,
                              const std::string& newsub, bool replace_all);
    
    // ----------------------------------------------------------------------
    // SplitStringUsing()
    //    Split a string using a character delimiter. Append the components
    //    to 'result'.  If there are consecutive delimiters, this function skips
    //    over all of them.
    // ----------------------------------------------------------------------
    void SplitStringUsing(const std::string& full, const char* delim,
                          std::vector<std::string>* res);
    
    // Split a string using one or more byte delimiters, presented
    // as a nul-terminated c string. Append the components to 'result'.
    // If there are consecutive delimiters, this function will return
    // corresponding empty strings.  If you want to drop the empty
    // strings, try SplitStringUsing().
    //
    // If "full" is the empty string, yields an empty string as the only value.
    // ----------------------------------------------------------------------
    void SplitStringAllowEmpty(const std::string& full,
                                                  const char* delim,
                               std::vector<std::string>* result);
    
    // ----------------------------------------------------------------------
    // Split()
    //    Split a string using a character delimiter.
    // ----------------------------------------------------------------------
    inline std::vector<std::string> Split(
                                          const std::string& full, const char* delim, bool skip_empty = true) {
        std::vector<std::string> result;
        if (skip_empty) {
            SplitStringUsing(full, delim, &result);
        } else {
            SplitStringAllowEmpty(full, delim, &result);
        }
        return result;
    }
    
    // ----------------------------------------------------------------------
    // JoinStrings()
    //    These methods concatenate a vector of strings into a C++ string, using
    //    the C-string "delim" as a separator between components. There are two
    //    flavors of the function, one flavor returns the concatenated string,
    //    another takes a pointer to the target string. In the latter case the
    //    target string is cleared and overwritten.
    // ----------------------------------------------------------------------
    void JoinStrings(const std::vector<std::string>& components,
                     const char* delim, std::string* result);
    
    inline std::string JoinStrings(const std::vector<std::string>& components,
                              const char* delim) {
        std::string result;
        JoinStrings(components, delim, &result);
        return result;
    }
    
    
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rename a bunch of common string functions so they are consistent across
    // platforms and between char and wchar_t variants.
    // Here is the full list of functions that are unified:
    //  strlen, strcmp, stricmp, strncmp, strnicmp
    //  strchr, vsnprintf, strtoul, tolowercase
    // tolowercase is like tolower, but not compatible with end-of-file value
    //
    // It's not clear if we will ever use wchar_t strings on unix.  In theory,
    // all strings should be Utf8 all the time, except when interfacing with Win32
    // APIs that require Utf16.
    ///////////////////////////////////////////////////////////////////////////////
    
    inline char tolowercase(char c)
    {
        return static_cast<char>(tolower(c));
    }
    
#if defined(WEBRTC_WIN)
    
    inline size_t strlen(const wchar_t* s)
    {
        return wcslen(s);
    }
    inline int strcmp(const wchar_t* s1, const wchar_t* s2)
    {
        return wcscmp(s1, s2);
    }
    inline int stricmp(const wchar_t* s1, const wchar_t* s2)
    {
        return _wcsicmp(s1, s2);
    }
    inline int strncmp(const wchar_t* s1, const wchar_t* s2, size_t n)
    {
        return wcsncmp(s1, s2, n);
    }
    inline int strnicmp(const wchar_t* s1, const wchar_t* s2, size_t n)
    {
        return _wcsnicmp(s1, s2, n);
    }
    inline const wchar_t* strchr(const wchar_t* s, wchar_t c)
    {
        return wcschr(s, c);
    }
    inline const wchar_t* strstr(const wchar_t* haystack, const wchar_t* needle)
    {
        return wcsstr(haystack, needle);
    }
#ifndef vsnprintf
    inline int vsnprintf(wchar_t* buf, size_t n, const wchar_t* fmt, va_list args)
    {
        return _vsnwprintf(buf, n, fmt, args);
    }
#endif // !vsnprintf
    inline unsigned long strtoul(const wchar_t* snum, wchar_t** end, int base)
    {
        return wcstoul(snum, end, base);
    }
    inline wchar_t tolowercase(wchar_t c)
    {
        return static_cast<wchar_t>(towlower(c));
    }
    
#endif  // WEBRTC_WIN
    
#if defined(WEBRTC_POSIX)
    
    inline int _stricmp(const char* s1, const char* s2)
    {
        return strcasecmp(s1, s2);
    }
    inline int _strnicmp(const char* s1, const char* s2, size_t n)
    {
        return strncasecmp(s1, s2, n);
    }
    
#endif // WEBRTC_POSIX
    
    ///////////////////////////////////////////////////////////////////////////////
    // Traits simplifies porting string functions to be CTYPE-agnostic
    ///////////////////////////////////////////////////////////////////////////////
    
    
    
    const size_t SIZE_UNKNOWN = static_cast<size_t>(-1);
    
    template<class CTYPE>
    struct Traits
    {
        // STL string type
        //typedef XXX string;
        // Null-terminated string
        //inline static const CTYPE* empty_str();
    };
    
    ///////////////////////////////////////////////////////////////////////////////
    // String utilities which work with char or wchar_t
    ///////////////////////////////////////////////////////////////////////////////
    
    template<class CTYPE>
    inline const CTYPE* nonnull(const CTYPE* str, const CTYPE* def_str = NULL)
    {
        return str ? str : (def_str ? def_str : Traits<CTYPE>::empty_str());
    }
    
    template<class CTYPE>
    const CTYPE* strchr(const CTYPE* str, const CTYPE* chs)
    {
        for (size_t i=0; str[i]; ++i)
        {
            for (size_t j=0; chs[j]; ++j)
            {
                if (str[i] == chs[j])
                {
                    return str + i;
                }
            }
        }
        return 0;
    }
    
    template<class CTYPE>
    const CTYPE* strchrn(const CTYPE* str, size_t slen, CTYPE ch)
    {
        for (size_t i=0; i<slen && str[i]; ++i)
        {
            if (str[i] == ch)
            {
                return str + i;
            }
        }
        return 0;
    }
    
    template<class CTYPE>
    size_t strlenn(const CTYPE* buffer, size_t buflen)
    {
        size_t bufpos = 0;
        while (buffer[bufpos] && (bufpos < buflen))
        {
            ++bufpos;
        }
        return bufpos;
    }
    
    // Safe versions of strncpy, strncat, snprintf and vsnprintf that always
    // null-terminate.
    
    template<class CTYPE>
    size_t strcpyn(CTYPE* buffer, size_t buflen,
                   const CTYPE* source, size_t srclen = SIZE_UNKNOWN)
    {
        if (buflen <= 0)
            return 0;
        
        if (srclen == SIZE_UNKNOWN)
        {
            srclen = strlenn(source, buflen - 1);
        }
        else if (srclen >= buflen)
        {
            srclen = buflen - 1;
        }
        memcpy(buffer, source, srclen * sizeof(CTYPE));
        buffer[srclen] = 0;
        return srclen;
    }
    
    template<class CTYPE>
    size_t strcatn(CTYPE* buffer, size_t buflen,
                   const CTYPE* source, size_t srclen = SIZE_UNKNOWN)
    {
        if (buflen <= 0)
            return 0;
        
        size_t bufpos = strlenn(buffer, buflen - 1);
        return bufpos + strcpyn(buffer + bufpos, buflen - bufpos, source, srclen);
    }
    
    // Some compilers (clang specifically) require vsprintfn be defined before
    // sprintfn.
    template<class CTYPE>
    size_t vsprintfn(CTYPE* buffer, size_t buflen, const CTYPE* format,
                     va_list args)
    {
        int len = vsnprintf(buffer, buflen, format, args);
        if ((len < 0) || (static_cast<size_t>(len) >= buflen))
        {
            len = static_cast<int>(buflen - 1);
            buffer[len] = 0;
        }
        return len;
    }
    
    template<class CTYPE>
    size_t sprintfn(CTYPE* buffer, size_t buflen, const CTYPE* format, ...);
    template<class CTYPE>
    size_t sprintfn(CTYPE* buffer, size_t buflen, const CTYPE* format, ...)
    {
        va_list args;
        va_start(args, format);
        size_t len = vsprintfn(buffer, buflen, format, args);
        va_end(args);
        return len;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Allow safe comparing and copying ascii (not UTF-8) with both wide and
    // non-wide character strings.
    ///////////////////////////////////////////////////////////////////////////////
    
    inline int asccmp(const char* s1, const char* s2)
    {
        return strcmp(s1, s2);
    }
    inline int ascicmp(const char* s1, const char* s2)
    {
        return _stricmp(s1, s2);
    }
    inline int ascncmp(const char* s1, const char* s2, size_t n)
    {
        return strncmp(s1, s2, n);
    }
    inline int ascnicmp(const char* s1, const char* s2, size_t n)
    {
        return _strnicmp(s1, s2, n);
    }
    inline size_t asccpyn(char* buffer, size_t buflen,
                          const char* source, size_t srclen = SIZE_UNKNOWN)
    {
        return strcpyn(buffer, buflen, source, srclen);
    }
    
#if defined(WEBRTC_WIN)
    
    typedef wchar_t(*CharacterTransformation)(wchar_t);
    inline wchar_t identity(wchar_t c)
    {
        return c;
    }
    int ascii_string_compare(const wchar_t* s1, const char* s2, size_t n,
                             CharacterTransformation transformation);
    
    inline int asccmp(const wchar_t* s1, const char* s2)
    {
        return ascii_string_compare(s1, s2, static_cast<size_t>(-1), identity);
    }
    inline int ascicmp(const wchar_t* s1, const char* s2)
    {
        return ascii_string_compare(s1, s2, static_cast<size_t>(-1), tolowercase);
    }
    inline int ascncmp(const wchar_t* s1, const char* s2, size_t n)
    {
        return ascii_string_compare(s1, s2, n, identity);
    }
    inline int ascnicmp(const wchar_t* s1, const char* s2, size_t n)
    {
        return ascii_string_compare(s1, s2, n, tolowercase);
    }
    size_t asccpyn(wchar_t* buffer, size_t buflen,
                   const char* source, size_t srclen = SIZE_UNKNOWN);
    
#endif  // WEBRTC_WIN
    
    ///////////////////////////////////////////////////////////////////////////////
    // Traits<char> specializations
    ///////////////////////////////////////////////////////////////////////////////
    
    template<>
    struct Traits<char>
    {
        typedef std::string string;
        inline static const char* empty_str()
        {
            return "";
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////////
    // Traits<wchar_t> specializations (Windows only, currently)
    ///////////////////////////////////////////////////////////////////////////////
    
#if defined(WEBRTC_WIN)
    
    template<>
    struct Traits<wchar_t>
    {
        typedef std::wstring string;
        inline static const wchar_t* Traits<wchar_t>::empty_str()
        {
            return L"";
        }
    };
    
#endif  // WEBRTC_WIN 
    
    // Replaces all occurrences of "search" with "replace".
    void replace_substrs(const char *search,
                         size_t search_len,
                         const char *replace,
                         size_t replace_len,
                         std::string *s);
    
    // True iff s1 starts with s2.
    bool starts_with(const char *s1, const char *s2);
    
    // True iff s1 ends with s2.
    bool ends_with(const char *s1, const char *s2);
    
    // Remove leading and trailing whitespaces.
    std::string string_trim(const std::string& s);

}  // namespace libcommon

#endif // WEBRTC_BASE_STRINGUTILS_H__
