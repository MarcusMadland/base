

#include "../../include/mapp/layer.hpp"
#include "../../include/mapp/app.hpp"

namespace mapp
{
	Layer::Layer(const char* name)
		: layerName(name)
	{
	}

	LayerStack::LayerStack()
		: layerInsertIndex(0)
	{}

	LayerStack::~LayerStack()
	{
		for (const auto& layer : layers)
		{
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		layers.emplace(layers.begin() + layerInsertIndex, layer);
		layerInsertIndex++;
		layer->onInit();
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		layers.emplace_back(overlay);
		overlay->onInit();
	}

	void LayerStack::popLayer(Layer* layer)
	{
		const auto it = std::find(layers.begin(),
			layers.begin() + layerInsertIndex, layer);

		if (it != layers.begin() + layerInsertIndex)
		{
			layer->onShutdown();
			layers.erase(it);
			layerInsertIndex--;
		}
	}

	void LayerStack::popOverlay(Layer* overlay)
	{
		const auto it = std::find(layers.begin() +
			layerInsertIndex, layers.end(), overlay);

		if (it != layers.end())
		{
			overlay->onShutdown();
			layers.erase(it);
		}
	}
}