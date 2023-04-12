#pragma once

#include "../include/mapp/window.hpp"

namespace mapp
{
	class WindowMac : public Window
	{
	public:
		WindowMac(const WindowParams& params);
		~WindowMac();

		virtual void onUpdate(const float& dt) override;
		virtual void* getNativeWindow() override;
		virtual void* getNativeDisplay() override;

	private:

	};
}
