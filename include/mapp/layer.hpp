

#pragma once

#include "event.hpp"

#include <vector>

namespace mapp 
{
	class Layer
	{
	public:
		explicit Layer(const char* name = "Layer");
		virtual ~Layer() = default;
		
		Layer(const Layer&) = default;
		Layer(Layer&&) = default;
		
		Layer& operator=(const Layer&) = default;
		Layer& operator=(Layer&&) = default;

		virtual void onInit() {}
		virtual void onShutdown() {}
		virtual void onUpdate(const float& dt) {}
		virtual void onPostUpdate(const float& dt) {}
		virtual void onEvent(Event& event) {}

		[[nodiscard]] const char* getName() const { return layerName; }

	protected:
		const char* layerName;
	};

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		LayerStack(const LayerStack&) = default;
		LayerStack(LayerStack&&) = default;

		LayerStack& operator=(const LayerStack&) = default;
		LayerStack& operator=(LayerStack&&) = default;

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }

	private:
		std::vector<Layer*> layers;
		uint32_t layerInsertIndex;
	};
}