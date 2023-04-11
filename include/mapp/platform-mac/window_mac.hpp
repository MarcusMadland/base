#pragma once

#include "../include/mapp/window.hpp"

namespace mapp
{
	class WindowMac : public Window
	{
	public:
		WindowMac(const char* name, uint32_t width, uint32_t height);
		~WindowMac();

		virtual void onUpdate(const float& dt) override;
		virtual void* getNativeWindow() override;
		virtual void* getNativeDisplay() override;

	private:

	};
}
