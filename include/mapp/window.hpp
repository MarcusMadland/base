#pragma once

#include "event.hpp"

#include <memory>
#include <string>

namespace mapp
{
	struct WindowParams
	{
		std::string title = "mapp window";
		uint32_t width = 1280;
		uint32_t height = 720;

		bool canResize = true;
		bool canClose = true;

		bool showBorder = true;
	};

	class Window
	{
	public:
		Window(const WindowParams& params);
		~Window();

		virtual void onUpdate(const float& dt) = 0;
       
		virtual bool setFullscreen(const bool enable) = 0;

		virtual [[nodiscard]] bool getIsFullscreen() = 0;

		virtual void* getNativeWindow() = 0;
		virtual void* getNativeDisplay() = 0;

		void setEventCallback(const std::function<void(Event&)>& callback);

		[[nodiscard]] WindowParams getParams() { return params; }

		static Window* create(const WindowParams& params);
       
	public:
		std::function<void(Event&)> eventCallback;

	protected:
		WindowParams params;
	};
}
