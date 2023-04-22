#pragma once

// Automatic Detect operating systems
#if !defined(MAPP_CUSTOM_PLATFORM_DETECTION)
#if defined(__linux__)
#define MAPP_PLATFORM_LINUX
#if defined(__ANDROID__)
#define MAPP_PLATFORM_ANDROID
#endif
#elif defined(_WIN32)
#define MAPP_PLATFORM_WIN32
#if defined(WINAPI_FAMILY)
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE_APP)
#define MAPP_PLATFORM_WIN32_PHONE
#endif
#endif
#elif defined(__APPLE__)
#if defined(IPHONE)
#define MAPP_PLATFORM_IOS
#elif defined(__MACH__)
#define MAPP_PLATFORM_COCOA
#endif
#elif defined(__EMSCRIPTEN__)
#define MAPP_PLATFORM_EMSCRIPTEN
#else
#error "Unable to determine operating system"
#endif
#endif