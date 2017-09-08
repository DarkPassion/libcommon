/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This file contains platform-specific typedefs and defines.
// Much of it is derived from Chromium's build/build_config.h.

#ifndef _UTIL_TYPEDEFS_H_
#define _UTIL_TYPEDEFS_H_

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__)
#define WEBRTC_ARCH_X86_FAMILY
#define WEBRTC_ARCH_X86_64
#define WEBRTC_ARCH_64_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(__aarch64__)
#define WEBRTC_ARCH_64_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(_M_IX86) || defined(__i386__)
#define WEBRTC_ARCH_X86_FAMILY
#define WEBRTC_ARCH_X86
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(__ARMEL__)
// TODO(ajm): We'd prefer to control platform defines here, but this is
// currently provided by the Android makefiles. Commented to avoid duplicate
// definition warnings.
//#define WEBRTC_ARCH_ARM
// TODO(ajm): Chromium uses the following two defines. Should we switch?
//#define WEBRTC_ARCH_ARM_FAMILY
//#define WEBRTC_ARCH_ARMEL
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(__MIPSEL__)
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(__pnacl__)
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#else
#error Please add support for your architecture in typedefs.h
#endif

#if !(defined(WEBRTC_ARCH_LITTLE_ENDIAN) ^ defined(WEBRTC_ARCH_BIG_ENDIAN))
#error Define either WEBRTC_ARCH_LITTLE_ENDIAN or WEBRTC_ARCH_BIG_ENDIAN
#endif

#if (defined(WEBRTC_ARCH_X86_FAMILY) && !defined(__SSE2__)) ||  \
(defined(WEBRTC_ARCH_ARM_V7) && !defined(WEBRTC_ARCH_ARM_NEON))
#define WEBRTC_CPU_DETECTION
#endif

#if !defined(_MSC_VER)
#include <stdint.h>
#else
// Define C99 equivalent types, since pre-2010 MSVC doesn't provide stdint.h.
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef __int64             int64_t;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned __int64    uint64_t;
#endif

// Borrowed from Chromium's base/compiler_specific.h.
// Annotate a virtual method indicating it must be overriding a virtual
// method in the parent class.
// Use like:
//   virtual void foo() OVERRIDE;
#if defined(_MSC_VER)
#define OVERRIDE override
#elif defined(__clang__)
// Clang defaults to C++03 and warns about using override. Squelch that.
// Intentionally no push/pop here so all users of OVERRIDE ignore the warning
// too. This is like passing -Wno-c++11-extensions, except that GCC won't die
// (because it won't see this pragma).
#pragma clang diagnostic ignored "-Wc++11-extensions"
#define OVERRIDE override
#elif defined(__GNUC__) && __cplusplus >= 201103 && \
(__GNUC__ * 10000 + __GNUC_MINOR__ * 100) >= 40700
// GCC 4.7 supports explicit virtual overrides when C++11 support is enabled.
#define OVERRIDE override
#else
#define OVERRIDE
#endif

// Annotate a function indicating the caller must examine the return value.
// Use like:
//   int foo() WARN_UNUSED_RESULT;
// TODO(ajm): Hack to avoid multiple definitions until the base/ of webrtc and
// libjingle are merged.
#if !defined(WARN_UNUSED_RESULT)
#if defined(__GNUC__)
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif
#endif  // WARN_UNUSED_RESULT

// Put after a variable that might not be used, to prevent compiler warnings:
//   int result ATTRIBUTE_UNUSED = DoSomething();
//   assert(result == 17);
#ifndef ATTRIBUTE_UNUSED
#if defined(__GNUC__) || defined(__clang__)
#define ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define ATTRIBUTE_UNUSED
#endif
#endif

// Annotate a function that will not return control flow to the caller.
#if defined(_MSC_VER)
#define NO_RETURN __declspec(noreturn)
#elif defined(__GNUC__)
#define NO_RETURN __attribute__((noreturn))
#else
#define NO_RETURN
#endif



#define ARRAY_SIZE(x) (static_cast<int>(sizeof(x) / sizeof(x[0])))

/////////////////////////////////////////////////////////////////////////////
// Assertions
/////////////////////////////////////////////////////////////////////////////

#ifndef ENABLE_DEBUG
#define ENABLE_DEBUG _DEBUG
#endif  // !defined(ENABLE_DEBUG)


// Even for release builds, allow for the override of LogAssert. Though no
// macro is provided, this can still be used for explicit runtime asserts
// and allow applications to override the assert behavior.

namespace libcommon {
    
    
    // If a debugger is attached, triggers a debugger breakpoint. If a debugger is
    // not attached, forces program termination.
    void Break();
    
    // LogAssert writes information about an assertion to the log. It's called by
    // Assert (and from the ASSERT macro in debug mode) before any other action
    // is taken (e.g. breaking the debugger, abort()ing, etc.).
    void LogAssert(const char* function, const char* file, int line,
                   const char* expression);
    
    typedef void (*AssertLogger)(const char* function,
    const char* file,
    int line,
    const char* expression);
    
    // Sets a custom assert logger to be used instead of the default LogAssert
    // behavior. To clear the custom assert logger, pass NULL for |logger| and the
    // default behavior will be restored. Only one custom assert logger can be set
    // at a time, so this should generally be set during application startup and
    // only by one component.
    void SetCustomAssertLogger(AssertLogger logger);
    
    bool IsOdd(int n);
    
    bool IsEven(int n);
    
}  // namespace libcommon

#if ENABLE_DEBUG

namespace libcommon {
    
    inline bool Assert(bool result, const char* function, const char* file,
                       int line, const char* expression) {
        if (!result) {
            LogAssert(function, file, line, expression);
            Break();
        }
        return result;
    }
    
    // Same as Assert above, but does not call Break().  Used in assert macros
    // that implement their own breaking.
    inline bool AssertNoBreak(bool result, const char* function, const char* file,
                              int line, const char* expression) {
        if (!result)
            LogAssert(function, file, line, expression);
        return result;
    }
    
}  // namespace libcommon

#if defined(_MSC_VER) && _MSC_VER < 1300
#define __FUNCTION__ ""
#endif

#ifndef ASSERT
#if defined(WIN32)
// Using debugbreak() inline on Windows directly in the ASSERT macro, has the
// benefit of breaking exactly where the failing expression is and not two
// calls up the stack.
#define ASSERT(x) \
(rtc::AssertNoBreak((x), __FUNCTION__, __FILE__, __LINE__, #x) ? \
(void)(1) : __debugbreak())
#else
#define ASSERT(x) \
(void)rtc::Assert((x), __FUNCTION__, __FILE__, __LINE__, #x)
#endif
#endif

#ifndef VERIFY
#if defined(WIN32)
#define VERIFY(x) \
(rtc::AssertNoBreak((x), __FUNCTION__, __FILE__, __LINE__, #x) ? \
true : (__debugbreak(), false))
#else
#define VERIFY(x) rtc::Assert((x), __FUNCTION__, __FILE__, __LINE__, #x)
#endif
#endif

#else  // !ENABLE_DEBUG

namespace libcommon {
    
    inline bool ImplicitCastToBool(bool result) { return result; }
    
}  // namespace libcommon

#ifndef ASSERT
#define ASSERT(x) (void)0
#endif

#ifndef VERIFY
#define VERIFY(x) rtc::ImplicitCastToBool(x)
#endif

#endif  // !ENABLE_DEBUG




#endif  // _UTIL_TYPEDEFS_H_
