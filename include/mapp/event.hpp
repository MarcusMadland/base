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

#include <functional>

namespace mapp {

enum class EventType
{
	None,

	WindowClose, 
	WindowResize, 

	KeyPressed,
	KeyReleased,
	KeyPressing,

	GamepadKeyPressed,
	GamepadKeyReleased,
	GamepadKeyPressing,
	GamepadLeftJoystick,
	GamepadRightJoystick,
	GamepadLeftTrigger,
	GamepadRightTrigger,

	MouseMoved,
	MouseScrolled,
	MouseButtonPressed,
	MouseButtonReleased,
};
	
class Event
{
public:	
	[[nodiscard]] virtual EventType getEventType() const = 0;

	[[nodiscard]] bool getIsHandled() { return mHandled; }
	void setIsHandled(bool handled) { mHandled = handled; }

private:
	bool mHandled = false;
};
	
class EventDispatcher
{
public:
	explicit EventDispatcher(Event& event);

	template<typename T>
	bool dispatch(std::function<bool(T&)> func)
	{
		if (mEvent.getEventType() == T().getEventType())
		{
			mEvent.setIsHandled(func(*static_cast<T*>(&mEvent)));
			return true;
		}
		return false;
	}

private:
	Event& mEvent;
};

class WindowResizeEvent final : public Event
{
public:
	explicit WindowResizeEvent(const uint32_t width = 0, const uint32_t height = 0);

	[[nodiscard]] uint32_t getWidth() const { return mWidth; }
	[[nodiscard]] uint32_t getHeight() const { return mHeight; }

		
	[[nodiscard]] EventType getEventType() const override { return EventType::WindowResize; }


private:
	uint32_t mWidth;
	uint32_t mHeight;
};
	
class WindowCloseEvent final : public Event
{
public:
	[[nodiscard]] EventType getEventType() const override { return EventType::WindowClose; }
};

class KeyEvent : public Event
{
public:
	explicit KeyEvent(uint64_t code = 0);

	[[nodiscard]] uint64_t getKeyCode() const { return mKeyCode; }

private:
	uint64_t mKeyCode;
};
	
// Key
class KeyPressedEvent final : public KeyEvent
{
public:
	explicit KeyPressedEvent(const uint64_t code = 0);

	[[nodiscard]] EventType getEventType() const override { return EventType::KeyPressed; }
};
	
class KeyReleasedEvent final : public KeyEvent
{
public:
	explicit KeyReleasedEvent(const uint64_t code = 0);

	[[nodiscard]] EventType getEventType() const override { return EventType::KeyReleased; }
};

class KeyPressingEvent final : public KeyEvent
{
public:
	explicit KeyPressingEvent(const uint64_t code = 0);

	[[nodiscard]] EventType getEventType() const override { return EventType::KeyPressing; }
};

// Gamepad
class GamepadKeyPressedEvent final : public KeyEvent
{
public:
	explicit GamepadKeyPressedEvent(const uint64_t code = 0, const int id = 0);

	[[nodiscard]] int getControlledID() const { return mControllerIndex; }

	[[nodiscard]] EventType getEventType() const override { return EventType::GamepadKeyPressed; }
private:
	int mControllerIndex;
};

class GamepadKeyReleasedEvent final : public KeyEvent
{
public:
	explicit GamepadKeyReleasedEvent(const uint64_t code = 0, const int id = 0);

	[[nodiscard]] int getControlledID() const { return mControllerIndex; }

	[[nodiscard]] EventType getEventType() const override { return EventType::GamepadKeyReleased; }

private:
	int mControllerIndex;
};

class GamepadKeyPressingEvent final : public KeyEvent
{
public:
	explicit GamepadKeyPressingEvent(const uint64_t code = 0, const int id = 0);

	[[nodiscard]] int getControlledID() const { return mControllerIndex; }

	[[nodiscard]] EventType getEventType() const override { return EventType::GamepadKeyPressing; }

private:
	int mControllerIndex;
};

class GamepadLeftJoystickEvent final : public Event
{
public:
	explicit GamepadLeftJoystickEvent(const float x = 0, const float y = 0, const int id = 0);

	[[nodiscard]] int getControlledID() const { return mControllerIndex; }

	[[nodiscard]] float getX() const { return mAxisX; }
	[[nodiscard]] float getY() const { return mAxisY; }

	[[nodiscard]] EventType getEventType() const override { return EventType::GamepadLeftJoystick; }

private:
	int mControllerIndex;
	float mAxisX, mAxisY;
};

class GamepadRightJoystickEvent final : public Event
{
public:
	explicit GamepadRightJoystickEvent(const float x = 0, const float y = 0, const int id = 0);

	[[nodiscard]] int getControlledID() const { return mControllerIndex; }

	[[nodiscard]] float getX() const { return mAxisX; }
	[[nodiscard]] float getY() const { return mAxisY; }

	[[nodiscard]] EventType getEventType() const override { return EventType::GamepadRightJoystick; }

private:
	int mControllerIndex;
	float mAxisX, mAxisY;
};

class GamepadLeftTriggerEvent final : public Event
{
public:
	explicit GamepadLeftTriggerEvent(const float x = 0, const int id = 0);

	[[nodiscard]] int getControlledID() const { return mControllerIndex; }

	[[nodiscard]] float getX() const { return mAxisX; }

	[[nodiscard]] EventType getEventType() const override { return EventType::GamepadLeftTrigger; }

private:
	int mControllerIndex;
	float mAxisX;
};

class GamepadRightTriggerEvent final : public Event
{
public:
	explicit GamepadRightTriggerEvent(const float x = 0, const int id = 0);

	[[nodiscard]] int getControlledID() const { return mControllerIndex; }

	[[nodiscard]] float getX() const { return mAxisX; }

	[[nodiscard]] EventType getEventType() const override { return EventType::GamepadRightTrigger; }

private:
	int mControllerIndex;
	float mAxisX;
};

// Mouse
class MouseMovedEvent final : public Event
{
public:
	explicit MouseMovedEvent(const float x = 0.0f, const float y = 0.0f);

	[[nodiscard]] float getX() const { return mMouseX; }
	[[nodiscard]] float getY() const { return mMouseY; }

	[[nodiscard]] EventType getEventType() const override { return EventType::MouseMoved; }
		
private:
	float mMouseX, mMouseY;
};
	
class MouseScrolledEvent final : public Event
{
public:
	explicit MouseScrolledEvent(const float yOffset = 0.0f);

	[[nodiscard]] float getYOffset() const { return mOffsetY; }

	[[nodiscard]] EventType getEventType() const override { return EventType::MouseScrolled; }
		
private:
	float mOffsetY;
};
	
class MouseButtonEvent : public Event
{
public:
	explicit MouseButtonEvent(const uint64_t keyCode = 0);
		
	[[nodiscard]] uint64_t getKeyCode() const { return mKeyCode; }
	
private:
	uint64_t mKeyCode;
};
	
class MouseButtonPressedEvent final : public MouseButtonEvent
{
public:
	explicit MouseButtonPressedEvent(const uint64_t keyCode = 0);

	[[nodiscard]] EventType getEventType() const override { return EventType::MouseButtonPressed; }
};
	
class MouseButtonReleasedEvent final : public MouseButtonEvent
{
public:
	explicit MouseButtonReleasedEvent(const uint64_t keyCode = 0);

	[[nodiscard]] EventType getEventType() const override { return EventType::MouseButtonReleased; }
};

}	// namespace mapp