

#pragma once

#include "window.hpp"
#include "event.hpp"
#include "layer.hpp"

namespace mapp 
{

	class App
	{
	public:
		explicit App(Window* window = nullptr);
		virtual ~App() = default;

		App(const App&) = delete;
		App(App&&) = delete;
		
		App& operator=(const App&) = delete;
		App& operator=(App&&) = delete;

		void run();
		void shutdown();
		void onEvent(Event& e);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		static App& getInstance() { return *instance; }
		[[nodiscard]] Window* getWindow() const { return window; }

	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(const WindowResizeEvent& e);

	private:
        static App* instance;

		Window* window; 
		LayerStack layerStack;
		bool isRunning;
        bool isMinimized;
		float lastFrameTime;
		float deltaTime;
	};
}