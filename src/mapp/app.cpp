#include "../../include/mapp/app.hpp"
#include <chrono>

namespace mapp
{
	#define BIND_EVENT_FN(x) std::bind(&App::x, \
		this, std::placeholders::_1)
	App* App::instance = nullptr;

	App::App(const AppParams& params)
		: isRunning(true)
		, isMinimized(false)
		, lastFrameTime(0.0f)
		, deltaTime(0.0f)
		, window(nullptr)
	{
		if (!instance)
		{
			// Initialize core
			
		}
		instance = this;
	}

	void App::pushLayer(Layer* layer)
	{
		layerStack.pushLayer(layer);
	}

	void App::pushOverlay(Layer* layer)
	{
		layerStack.pushOverlay(layer);
	}

	void App::run()
	{
		while (isRunning)
		{
			const auto start = std::chrono::high_resolution_clock::now(); 

			if (!isMinimized)
			{
				for (Layer* layer : layerStack)
				{
					layer->onUpdate(deltaTime);
				}

				if (window)
				{
					window->onUpdate(deltaTime);
				}

				for (Layer* layer : layerStack)
				{
					layer->onPostUpdate(deltaTime);
				}
			}

			const auto end = std::chrono::high_resolution_clock::now();
			const std::chrono::duration<float> duration = end - start;

			lastFrameTime = deltaTime;
			deltaTime = duration.count();
		}
	}

	void App::shutdown()
	{
		if (isRunning)
		{
			isRunning = false;
		}
		delete window;
	}

	void App::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

		for (auto it = layerStack.end(); it != layerStack.begin(); )
		{
			(*--it)->onEvent(e);
			if (e.handled)
				break;
		}
	}

	bool App::onWindowClose(WindowCloseEvent& e)
	{
		isRunning = false;
		return true;
	}

	bool App::onWindowResize(const WindowResizeEvent& e)
	{
		isMinimized = (e.getWidth() == 0 || e.getHeight() == 0);
		return false;
	}
}