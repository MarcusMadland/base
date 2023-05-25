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

#include "mapp/app.hpp"

#include <chrono>
#include <iostream>
#include <cassert>

namespace mapp {

void AppContext::setApp(App* app)
{
	assert(app);
	mApp = app;
}
void AppContext::setWindow(Window* window)
{
	assert(window);
	mWindow = window;
}

App::App(Window* window) 
	: mContext(AppContext())
	, mLayerStack(LayerStack())
	, mWindow(window)
	, mIsRunning(true)
	, mLastFrameTime(0.0f)
	, mDeltaTime(0.0f)
{
	mContext.setApp(this);

	if (mWindow) 
	{
		mContext.setWindow(mWindow);
        mWindow->setEventCallback([this](Event& event) { onEvent(event); });
	}
}

void App::pushLayer(Layer* layer)
{
	assert(layer);
	mLayerStack.pushLayer(layer, mContext);
}

void App::pushOverlay(Layer* layer)
{
	assert(layer);
	mLayerStack.pushOverlay(layer, mContext);
}

void App::run()
{
	while (mIsRunning)
	{
		const auto start = std::chrono::high_resolution_clock::now(); 
			
		if (mWindow)
		{
			mWindow->onUpdate(mDeltaTime);
		}

		for (Layer* layer : mLayerStack)
		{
			assert(layer);
			layer->onUpdate(mDeltaTime);
		}

		for (Layer* layer : mLayerStack)
		{
			assert(layer);
			layer->onRender(mDeltaTime);
		}

		for (Layer* layer : mLayerStack)
		{
			assert(layer);
			layer->onPostRender(mDeltaTime);
		}
			

		const auto end = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> duration = end - start;

		mLastFrameTime = mDeltaTime;
		mDeltaTime = duration.count();
	}
}

void App::shutdown()
{
	if (mIsRunning)
	{
		mIsRunning = false;
	}
}

void App::onEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<WindowCloseEvent>(std::bind(&App::onWindowClose, this, std::placeholders::_1));
	dispatcher.dispatch<WindowResizeEvent>(std::bind(&App::onWindowResize, this, std::placeholders::_1));

	for (auto it = mLayerStack.end(); it != mLayerStack.begin(); )
	{
		(*--it)->onEvent(event);
		if (event.getIsHandled())
		{
			break;
		}
	}
}

bool App::onWindowClose(WindowCloseEvent& event)
{
	mIsRunning = false;
	return true;
}

bool App::onWindowResize(const WindowResizeEvent& event)
{
	return false;
}

}	// namespace mapp
