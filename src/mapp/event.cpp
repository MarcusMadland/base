#include "../../include/mapp/event.hpp"

namespace mapp
{
	EventDispatcher::EventDispatcher(Event& event)
		: event(event)
	{}

	WindowResizeEvent::WindowResizeEvent(const uint32_t width, const uint32_t height)
		: width(width), height(height)
	{}

	KeyEvent::KeyEvent(const uint64_t code)
		 : keyCode(code)
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
	
	MouseMovedEvent::MouseMovedEvent(const float x, const float y)
		 : mouseX(x), mouseY(y)
	{}

	MouseScrolledEvent::MouseScrolledEvent(const float xOffset, const float yOffset)
		: offsetX(xOffset), offsetY(yOffset)
	{}

	MouseButtonEvent::MouseButtonEvent(const uint64_t button)
		: button(button)
	{}

	MouseButtonPressedEvent::MouseButtonPressedEvent(const uint64_t button)
		: MouseButtonEvent(button)
	{}
	
	MouseButtonReleasedEvent::MouseButtonReleasedEvent(const uint64_t button)
		: MouseButtonEvent(button)
	{}
	


}