/*
 * Copyright 2022 Marcus Madland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mapp/layer.hpp"
#include "mapp/app.hpp"
#include "mapp/event.hpp"

#include <cassert>

namespace mapp {

LayerStack::LayerStack()
	: mLayerInsertIndex(0)
{}

LayerStack::~LayerStack()
{
	for (Layer* layer : mLayers)
	{
		delete layer;
	}
}

void LayerStack::pushLayer(Layer* layer, AppContext& context)
{
	assert(layer);
	mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
	mLayerInsertIndex++;
	layer->onInit(context);
}

void LayerStack::pushOverlay(Layer* overlay, AppContext& context)
{
	assert(overlay);
	mLayers.emplace_back(overlay);
	overlay->onInit(context);
}

void LayerStack::popLayer(Layer* layer)
{
	assert(layer);
	const auto it = std::find(mLayers.begin(),
		mLayers.begin() + mLayerInsertIndex, layer);

	if (it != mLayers.begin() + mLayerInsertIndex)
	{
		layer->onShutdown();
		mLayers.erase(it);
		mLayerInsertIndex--;
	}
}

void LayerStack::popOverlay(Layer* overlay)
{
	assert(overlay);
	const auto it = std::find(mLayers.begin() +
		mLayerInsertIndex, mLayers.end(), overlay);

	if (it != mLayers.end())
	{
		overlay->onShutdown();
		mLayers.erase(it);
	}
}

}	// namespace mapp