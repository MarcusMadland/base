#include "../../include/mapp/event.hpp"

namespace mapp
{
	EventDispatcher::EventDispatcher(Event& event)
		: event(event)
	{}

	WindowResizeEvent::WindowResizeEvent(const uint32_t width, const uint32_t height)
		: width(width), height(height)
	{}

	KeyEvent::KeyEvent(const int code)
		 : keyCode(code)
	{}

	KeyPressedEvent::KeyPressedEvent(const int code, const int repeatCount)
		: KeyEvent(code), repeatCount(repeatCount)
	{}

	KeyReleasedEvent::KeyReleasedEvent(const int code)
		: KeyEvent(code)
	{}

	KeyTypedEvent::KeyTypedEvent(const int keycode)
		: KeyEvent(keycode)
	{}
	
	MouseMovedEvent::MouseMovedEvent(const float x, const float y)
		 : mouseX(x), mouseY(y)
	{}

	MouseScrolledEvent::MouseScrolledEvent(const float xOffset, const float yOffset)
		: offsetX(xOffset), offsetY(yOffset)
	{}

	MouseButtonEvent::MouseButtonEvent(const int button)
		: button(button)
	{}

	MouseButtonPressedEvent::MouseButtonPressedEvent(const int button)
		: MouseButtonEvent(button)
	{}
	
	MouseButtonReleasedEvent::MouseButtonReleasedEvent(const int button)
		: MouseButtonEvent(button)
	{}
	
}