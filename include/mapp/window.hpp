


#pragma once

#include <memory>

#include "event.hpp"

namespace mapp
{
	class Window
	{
	public:
		Window(const char* name, uint32_t width, uint32_t height);
		~Window();

		virtual void onUpdate(const float& dt) = 0;
        virtual void* getNativeWindow() = 0; 
		virtual void* getNativeDisplay() = 0;

		void setEventCallback(const std::function<void(Event&)>& callback);

		[[nodiscard]] const char* getName() const { return name; }
		[[nodiscard]] uint32_t getWidth() const { return width; }
		[[nodiscard]] uint32_t getHeight() const { return height; }
       
	protected:
        const char* name;
        uint32_t width;
        uint32_t height;
        std::function<void(Event&)> eventCallback;
	};
}