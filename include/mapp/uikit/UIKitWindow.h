#pragma once

#include "mapp/common/window_desc.hpp"
#include "mapp/common/init.hpp"
#include "mapp/common/event_queue.hpp"

namespace mapp
{
class Window
{
public:
	Window();
	
	~Window();
	
	bool update();
	
	enum class LayerType
	{
		Metal,
		OpenGL,
		LayerTypeMax
	};
	
	// Set the type of this window's view layer
	void setLayer(LayerType type);
	
	bool create(const WindowDesc& desc, EventQueue& eventQueue);
	
	void close();
	
	mapp::WindowDesc getDesc();
	
    void* getWindowHandle();
	
	//Any Layer Type
	void* layer;

	//XWinWindow*
	void* window;
	
	//XWinView*
	void* view;
	
	//XWinViewController*
	void* viewController;
	
	WindowDesc mDesc;
	
	mapp::EventQueue* mEventQueue;
	
};
}
