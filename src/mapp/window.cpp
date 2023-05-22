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

#include "mapp/window.hpp"
#include "mapp/platform.hpp"

#ifdef MAPP_PLATFORM_WIN32
#include "mapp/win32/win32_window.hpp"
#endif

#ifdef MAPP_PLATFORM_COCOA
#include "mapp/cocoa/cocoa_window.hpp"
#endif

namespace mapp {

Window::Window(const WindowParams& params)
	: mParams(params)
{}

void Window::setEventCallback(const std::function<void(Event&)>& callback)
{
	mEventCallback = callback;
}

Window* Window::create(const WindowParams& params)
{
	// Create window depending on platform
#ifdef MAPP_PLATFORM_WIN32
	return new WindowWin32(params);
#endif
        
#ifdef MAPP_PLATFORM_COCOA
	return new WindowCocoa(params);
#endif
        
    return nullptr;
}

}	// namespace mapp
