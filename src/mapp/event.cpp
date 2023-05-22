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

#include "mapp/event.hpp"

namespace mapp {

EventDispatcher::EventDispatcher(Event& event)
	: mEvent(event)
{}

WindowResizeEvent::WindowResizeEvent(const uint32_t width, const uint32_t height)
	: mWidth(width), mHeight(height)
{}

KeyEvent::KeyEvent(const uint64_t code)
	: mKeyCode(code)
{}

KeyPressedEvent::KeyPressedEvent(const uint64_t code)
	: KeyEvent(code)
{}

KeyReleasedEvent::KeyReleasedEvent(const uint64_t code)
	: KeyEvent(code)
{}

KeyPressingEvent::KeyPressingEvent(const uint64_t code)
	: KeyEvent(code)
{}

GamepadKeyPressedEvent::GamepadKeyPressedEvent(const uint64_t code, const int id)
	: KeyEvent(code), mControllerIndex(id)
{}

GamepadKeyReleasedEvent::GamepadKeyReleasedEvent(const uint64_t code, const int id)
	: KeyEvent(code), mControllerIndex(id)
{}

GamepadKeyPressingEvent::GamepadKeyPressingEvent(const uint64_t code, const int id)
	: KeyEvent(code), mControllerIndex(id)
{}

GamepadLeftJoystickEvent::GamepadLeftJoystickEvent(const float x, const float y, const int id)
	: mAxisX(x), mAxisY(y), mControllerIndex(id)
{}

GamepadRightJoystickEvent::GamepadRightJoystickEvent(const float x, const float y, const int id)
	: mAxisX(x), mAxisY(y), mControllerIndex(id)
{}
	
GamepadLeftTriggerEvent::GamepadLeftTriggerEvent(const float x, const int id)
	: mAxisX(x), mControllerIndex(id)
{}

GamepadRightTriggerEvent::GamepadRightTriggerEvent(const float x, const int id)
	: mAxisX(x), mControllerIndex(id)
{}

MouseMovedEvent::MouseMovedEvent(const float x, const float y)
	: mMouseX(x), mMouseY(y)
{}

MouseScrolledEvent::MouseScrolledEvent(const float yOffset)
	: mOffsetY(yOffset)
{}

MouseButtonEvent::MouseButtonEvent(const uint64_t keyCode)
	: mKeyCode(keyCode)
{}

MouseButtonPressedEvent::MouseButtonPressedEvent(const uint64_t keyCode)
	: MouseButtonEvent(keyCode)
{}
	
MouseButtonReleasedEvent::MouseButtonReleasedEvent(const uint64_t keyCode)
	: MouseButtonEvent(keyCode)
{}

}	// namespace mapp