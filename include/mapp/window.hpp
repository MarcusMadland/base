


#pragma once

#include <memory>

#include "event.hpp"

namespace mapp
{
	class Window
	{
	private:
		struct WindowInfo
		{
			WindowInfo()
				: title("window")
				, width(1280)
				, height(720)
			{}

			const char* title;
			uint32_t width;
			uint32_t height;

			std::function<void(Event&)> eventCallback;
		};

	public:
		Window(const char* name, uint32_t width, uint32_t height);
		~Window();

		virtual void onInit() {}
		virtual void onShutdown() {}
		virtual void onUpdate(const float& dt) {}

		void setEventCallback(const std::function<void(Event&)>& callback);

		[[nodiscard]] uint32_t getWidth() const { return windowInfo.width; }
		[[nodiscard]] uint32_t getHeight() const { return windowInfo.height; }

	private:
		WindowInfo windowInfo;
	};
}