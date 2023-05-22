/*
 * Copyright 2022 Marcus Madland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

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