

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
		KeyTyped,

		MouseMoved,
		MouseScrolled,
		MouseButtonPressed,
		MouseButtonReleased,
	};
	
	#define EVENT_CLASS_TYPE(type) \
		static EventType getStaticType() { return EventType::##type; } \
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
		explicit KeyEvent(int code);

		[[nodiscard]] int getKeyCode() const { return keyCode; }

	private:
		int keyCode;
	};
	
	class KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(const int code, const int repeatCount);

		[[nodiscard]] int getRepeatCount() const { return repeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int repeatCount;
	};
	
	class KeyReleasedEvent final : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent(const int code);

		EVENT_CLASS_TYPE(KeyReleased)
	};
	
	class KeyTypedEvent final : public KeyEvent
	{
	public:
		explicit KeyTypedEvent(const int keycode);

		EVENT_CLASS_TYPE(KeyTyped)
	};

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
		MouseScrolledEvent(const float xOffset, const float yOffset);

		[[nodiscard]] float getXOffset() const { return offsetX; }
		[[nodiscard]] float getYOffset() const { return offsetY; }

		EVENT_CLASS_TYPE(MouseScrolled)
		
	private:
		float offsetX, offsetY;
	};
	
	class MouseButtonEvent : public Event
	{
	public:
		explicit MouseButtonEvent(const int button);
		
		[[nodiscard]] int getMouseButton() const { return button; }
	
	private:
		int button;
	};
	
	class MouseButtonPressedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressedEvent(const int button);

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};
	
	class MouseButtonReleasedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleasedEvent(const int button);

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}