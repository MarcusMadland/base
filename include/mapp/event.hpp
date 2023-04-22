

#pragma once

#include <functional>

namespace mapp
{
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
	
	#define EVENT_CLASS_TYPE(type) \
		static EventType getStaticType() { return EventType::type; } \
		virtual EventType getEventType() const override { return getStaticType(); } 
	
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;
		
		Event(const Event&) = default;
		Event(Event&&) = default;
		
		Event& operator=(const Event&) = default;
		Event& operator=(Event&&) = default;
		
		
		[[nodiscard]] virtual EventType getEventType() const = 0;

		
		bool handled = false;
	};
	
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		explicit EventDispatcher(Event& event);

		
		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			if (event.getEventType() == T::getStaticType())
			{
				event.handled = func(*static_cast<T*>(&event));
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};

	class WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(const uint32_t width, const uint32_t height);

		[[nodiscard]] uint32_t getWidth() const { return width; }
		[[nodiscard]] uint32_t getHeight() const { return height; }

		
		EVENT_CLASS_TYPE(WindowResize)

	private:
		uint32_t width;
		uint32_t height;
	};
	
	class WindowCloseEvent final : public Event
	{
	public:
		EVENT_CLASS_TYPE(WindowClose)
	};

	class KeyEvent : public Event
	{
	public:
		explicit KeyEvent(uint64_t code);

		[[nodiscard]] uint64_t getKeyCode() const { return keyCode; }

	private:
		uint64_t keyCode;
	};
	
	// Key
	class KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(const uint64_t code);

		EVENT_CLASS_TYPE(KeyPressed)
	};
	
	class KeyReleasedEvent final : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent(const uint64_t code);

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyPressingEvent final : public KeyEvent
	{
	public:
		KeyPressingEvent(const uint64_t code);

		EVENT_CLASS_TYPE(KeyPressing)
	};

	// Gamepad
	class GamepadKeyPressedEvent final : public KeyEvent
	{
	public:
		GamepadKeyPressedEvent(const uint64_t code, const int id);

		[[nodiscard]] int getControlledID() const { return ControllerIndex; }

		EVENT_CLASS_TYPE(GamepadKeyPressed)
	private:
		int ControllerIndex;
	};

	class GamepadKeyReleasedEvent final : public KeyEvent
	{
	public:
		explicit GamepadKeyReleasedEvent(const uint64_t code, const int id);

		[[nodiscard]] int getControlledID() const { return ControllerIndex; }

		EVENT_CLASS_TYPE(GamepadKeyReleased)

	private:
		int ControllerIndex;
	};

	class GamepadKeyPressingEvent final : public KeyEvent
	{
	public:
		GamepadKeyPressingEvent(const uint64_t code, const int id);

		[[nodiscard]] int getControlledID() const { return ControllerIndex; }

		EVENT_CLASS_TYPE(GamepadKeyPressing)

	private:
		int ControllerIndex;
	};

	class GamepadLeftJoystickEvent final : public Event
	{
	public:
		GamepadLeftJoystickEvent(const float x, const float y, const int id);

		[[nodiscard]] int getControlledID() const { return ControllerIndex; }

		[[nodiscard]] float getX() const { return axisX; }
		[[nodiscard]] float getY() const { return axisY; }

		EVENT_CLASS_TYPE(GamepadLeftJoystick)

	private:
		int ControllerIndex;
		float axisX, axisY;
	};

	class GamepadRightJoystickEvent final : public Event
	{
	public:
		GamepadRightJoystickEvent(const float x, const float y, const int id);

		[[nodiscard]] int getControlledID() const { return ControllerIndex; }

		[[nodiscard]] float getX() const { return axisX; }
		[[nodiscard]] float getY() const { return axisY; }

		EVENT_CLASS_TYPE(GamepadRightJoystick)

	private:
		int ControllerIndex;
		float axisX, axisY;
	};

	class GamepadLeftTriggerEvent final : public Event
	{
	public:
		GamepadLeftTriggerEvent(const float x, const int id);

		[[nodiscard]] int getControlledID() const { return ControllerIndex; }

		[[nodiscard]] float getX() const { return axisX; }

		EVENT_CLASS_TYPE(GamepadLeftTrigger)

	private:
		int ControllerIndex;
		float axisX;
	};

	class GamepadRightTriggerEvent final : public Event
	{
	public:
		GamepadRightTriggerEvent(const float x, const int id);

		[[nodiscard]] int getControlledID() const { return ControllerIndex; }

		[[nodiscard]] float getX() const { return axisX; }

		EVENT_CLASS_TYPE(GamepadRightTrigger)

	private:
		int ControllerIndex;
		float axisX;
	};

	// Mouse
	class MouseMovedEvent final : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y);

		[[nodiscard]] float getX() const { return mouseX; }
		[[nodiscard]] float getY() const { return mouseY; }

		EVENT_CLASS_TYPE(MouseMoved)
		
	private:
		float mouseX, mouseY;
	};
	
	class MouseScrolledEvent final : public Event
	{
	public:
		MouseScrolledEvent(const float yOffset);

		[[nodiscard]] float getYOffset() const { return offsetY; }

		EVENT_CLASS_TYPE(MouseScrolled)
		
	private:
		float offsetY;
	};
	
	class MouseButtonEvent : public Event
	{
	public:
		explicit MouseButtonEvent(const uint64_t button);
		
		[[nodiscard]] uint64_t getMouseButton() const { return button; }
	
	private:
		uint64_t button;
	};
	
	class MouseButtonPressedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressedEvent(const uint64_t button);

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};
	
	class MouseButtonReleasedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleasedEvent(const uint64_t button);

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}