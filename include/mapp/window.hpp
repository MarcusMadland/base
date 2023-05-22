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

#include "event.hpp"

#include <memory>
#include <string>

namespace mapp {

struct WindowParams
{
	std::string mTitle = "mapp window";
	uint32_t mWidth = 1280;
	uint32_t mHeight = 720;

	bool mCanResize = true;
	bool mCanClose = true;

	bool mShowBorder = true;
};

class Window
{
public:
	Window(const WindowParams& params);

	virtual void onUpdate(const float& dt) = 0;
       
	virtual bool setFullscreen(const bool enable) = 0;
	[[nodiscard]] virtual bool getIsFullscreen() = 0;

	virtual void* getNativeWindow() = 0;
	virtual void* getNativeDisplay() = 0;

	void setEventCallback(const std::function<void(Event&)>& callback);

	[[nodiscard]] WindowParams getParams() { return mParams; }

	static Window* create(const WindowParams& params);
       
public:
	std::function<void(Event&)> mEventCallback;

private:
	WindowParams mParams;
};

}	// namespace mapp
