/*
 *  Copyright 2004 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "string/stringutils.h"

namespace libcommon
{
    
    bool memory_check(const void* memory, int c, size_t count)
    {
        const char* char_memory = static_cast<const char*>(memory);
        char char_c = static_cast<char>(c);
        for (size_t i = 0; i < count; ++i)
        {
            if (char_memory[i] != char_c)
            {
                return false;
            }
        }
        return true;
    }
    
    bool string_match(const char* target, const char* pattern)
    {
        while (*pattern)
        {
            if (*pattern == '*')
            {
                if (!*++pattern)
                {
                    return true;
                }
                while (*target)
                {
                    if ((toupper(*pattern) == toupper(*target))
                        && string_match(target + 1, pattern + 1))
                    {
                        return true;
                    }
                    ++target;
                }
                return false;
            }
            else
            {
                if (toupper(*pattern) != toupper(*target))
                {
                    return false;
                }
                ++target;
                ++pattern;
            }
        }
        return !*target;
    }
    
#if defined(WEBRTC_WIN)
    int ascii_string_compare(const wchar_t* s1, const char* s2, size_t n,
                             CharacterTransformation transformation)
    {
        wchar_t c1, c2;
        while (true)
        {
            if (n-- == 0) return 0;
            c1 = transformation(*s1);
            // Double check that characters are not UTF-8
            DCHECK_LT(static_cast<unsigned char>(*s2), 128);
            // Note: *s2 gets implicitly promoted to wchar_t
            c2 = transformation(*s2);
            if (c1 != c2) return (c1 < c2) ? -1 : 1;
            if (!c1) return 0;
            ++s1;
            ++s2;
        }
    }
    
    size_t asccpyn(wchar_t* buffer, size_t buflen,
                   const char* source, size_t srclen)
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
#if _DEBUG
        // Double check that characters are not UTF-8
        for (size_t pos = 0; pos < srclen; ++pos)
            DCHECK_LT(static_cast<unsigned char>(source[pos]), 128);
#endif  // _DEBUG
        std::copy(source, source + srclen, buffer);
        buffer[srclen] = 0;
        return srclen;
    }
    
#endif  // WEBRTC_WIN
    
    void replace_substrs(const char *search,
                         size_t search_len,
                         const char *replace,
                         size_t replace_len,
                         std::string *s)
    {
        size_t pos = 0;
        while ((pos = s->find(search, pos, search_len)) != std::string::npos)
        {
            s->replace(pos, search_len, replace, replace_len);
            pos += replace_len;
        }
    }
    
    bool starts_with(const char *s1, const char *s2)
    {
        return strncmp(s1, s2, strlen(s2)) == 0;
    }
    
    bool ends_with(const char *s1, const char *s2)
    {
        size_t s1_length = strlen(s1);
        size_t s2_length = strlen(s2);
        
        if (s2_length > s1_length)
        {
            return false;
        }
        
        const char* start = s1 + (s1_length - s2_length);
        return strncmp(start, s2, s2_length) == 0;
    }
    
    static const char kWhitespace[] = " \n\r\t";
    
    std::string string_trim(const std::string& s)
    {
        std::string::size_type first = s.find_first_not_of(kWhitespace);
        std::string::size_type last  = s.find_last_not_of(kWhitespace);
        
        if (first == std::string::npos || last == std::string::npos)
        {
            return std::string("");
        }
        
        return s.substr(first, last - first + 1);
    }
    
    using std::string;
    using std::vector;
    
    // ----------------------------------------------------------------------
    // ReplaceCharacters
    //    Replaces any occurrence of the character 'remove' (or the characters
    //    in 'remove') with the character 'replacewith'.
    // ----------------------------------------------------------------------
    void ReplaceCharacters(string *s, const char *remove, char replacewith) {
        const char *str_start = s->c_str();
        const char *str = str_start;
        for (str = strpbrk(str, remove);
             str != NULL;
             str = strpbrk(str + 1, remove)) {
            (*s)[str - str_start] = replacewith;
        }
    }
    
    void StripWhitespace(string* str) {
        int str_length = str->length();
        
        // Strip off leading whitespace.
        int first = 0;
        while (first < str_length && ascii_isspace(str->at(first))) {
            ++first;
        }
        // If entire string is white space.
        if (first == str_length) {
            str->clear();
            return;
        }
        if (first > 0) {
            str->erase(0, first);
            str_length -= first;
        }
        
        // Strip off trailing whitespace.
        int last = str_length - 1;
        while (last >= 0 && ascii_isspace(str->at(last))) {
            --last;
        }
        if (last != (str_length - 1) && last >= 0) {
            str->erase(last + 1, string::npos);
        }
    }
    
    
    // ----------------------------------------------------------------------
    // StringReplace()
    //    Replace the "old" pattern with the "new" pattern in a string,
    //    and append the result to "res".  If replace_all is false,
    //    it only replaces the first instance of "old."
    // ----------------------------------------------------------------------
    
    void StringReplace(const string& s, const string& oldsub,
                       const string& newsub, bool replace_all,
                       string* res) {
        if (oldsub.empty()) {
            res->append(s);  // if empty, append the given string.
            return;
        }
        
        string::size_type start_pos = 0;
        string::size_type pos;
        do {
            pos = s.find(oldsub, start_pos);
            if (pos == string::npos) {
                break;
            }
            res->append(s, start_pos, pos - start_pos);
            res->append(newsub);
            start_pos = pos + oldsub.size();  // start searching again after the "old"
        } while (replace_all);
        res->append(s, start_pos, s.length() - start_pos);
    }
    
    // ----------------------------------------------------------------------
    // StringReplace()
    //    Give me a string and two patterns "old" and "new", and I replace
    //    the first instance of "old" in the string with "new", if it
    //    exists.  If "global" is true; call this repeatedly until it
    //    fails.  RETURN a new string, regardless of whether the replacement
    //    happened or not.
    // ----------------------------------------------------------------------
    
    string StringReplace(const string& s, const string& oldsub,
                         const string& newsub, bool replace_all) {
        string ret;
        StringReplace(s, oldsub, newsub, replace_all, &ret);
        return ret;
    }
    
    
    
    // ----------------------------------------------------------------------
    // SplitStringUsing()
    //    Split a string using a character delimiter. Append the components
    //    to 'result'.
    //
    // Note: For multi-character delimiters, this routine will split on *ANY* of
    // the characters in the string, not the entire string as a single delimiter.
    // ----------------------------------------------------------------------
    template <typename ITR>
    static inline
    void SplitStringToIteratorUsing(const string& full,
                                    const char* delim,
                                    ITR& result) {
        // Optimize the common case where delim is a single character.
        if (delim[0] != '\0' && delim[1] == '\0') {
            char c = delim[0];
            const char* p = full.data();
            const char* end = p + full.size();
            while (p != end) {
                if (*p == c) {
                    ++p;
                } else {
                    const char* start = p;
                    while (++p != end && *p != c);
                    *result++ = string(start, p - start);
                }
            }
            return;
        }
        
        string::size_type begin_index, end_index;
        begin_index = full.find_first_not_of(delim);
        while (begin_index != string::npos) {
            end_index = full.find_first_of(delim, begin_index);
            if (end_index == string::npos) {
                *result++ = full.substr(begin_index);
                return;
            }
            *result++ = full.substr(begin_index, (end_index - begin_index));
            begin_index = full.find_first_not_of(delim, end_index);
        }
    }
    
    void SplitStringUsing(const string& full,
                          const char* delim,
                          vector<string>* result) {
        std::back_insert_iterator< vector<string> > it(*result);
        SplitStringToIteratorUsing(full, delim, it);
    }
    
    
    // Split a string using a character delimiter. Append the components
    // to 'result'.  If there are consecutive delimiters, this function
    // will return corresponding empty strings. The string is split into
    // at most the specified number of pieces greedily. This means that the
    // last piece may possibly be split further. To split into as many pieces
    // as possible, specify 0 as the number of pieces.
    //
    // If "full" is the empty string, yields an empty string as the only value.
    //
    // If "pieces" is negative for some reason, it returns the whole string
    // ----------------------------------------------------------------------
    template <typename StringType, typename ITR>
    static inline
    void SplitStringToIteratorAllowEmpty(const StringType& full,
                                         const char* delim,
                                         int pieces,
                                         ITR& result) {
        string::size_type begin_index, end_index;
        begin_index = 0;
        
        for (int i = 0; (i < pieces-1) || (pieces == 0); i++) {
            end_index = full.find_first_of(delim, begin_index);
            if (end_index == string::npos) {
                *result++ = full.substr(begin_index);
                return;
            }
            *result++ = full.substr(begin_index, (end_index - begin_index));
            begin_index = end_index + 1;
        }
        *result++ = full.substr(begin_index);
    }
    
    void SplitStringAllowEmpty(const string& full, const char* delim,
                               vector<string>* result) {
        std::back_insert_iterator<vector<string> > it(*result);
        SplitStringToIteratorAllowEmpty(full, delim, 0, it);
    }
    
    
    // ----------------------------------------------------------------------
    // JoinStrings()
    //    This merges a vector of string components with delim inserted
    //    as separaters between components.
    //
    // ----------------------------------------------------------------------
    template <class ITERATOR>
    static void JoinStringsIterator(const ITERATOR& start,
                                    const ITERATOR& end,
                                    const char* delim,
                                    string* result) {
        result->clear();
        int delim_length = strlen(delim);
        
        // Precompute resulting length so we can reserve() memory in one shot.
        int length = 0;
        for (ITERATOR iter = start; iter != end; ++iter) {
            if (iter != start) {
                length += delim_length;
            }
            length += iter->size();
        }
        result->reserve(length);
        
        // Now combine everything.
        for (ITERATOR iter = start; iter != end; ++iter) {
            if (iter != start) {
                result->append(delim, delim_length);
            }
            result->append(iter->data(), iter->size());
        }
    }
    
    void JoinStrings(const vector<string>& components,
                     const char* delim,
                     string * result) {
        JoinStringsIterator(components.begin(), components.end(), delim, result);
    }

}  // namespace libcommon
