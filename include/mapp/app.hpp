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

#pragma once

#include "window.hpp"
#include "event.hpp"
#include "layer.hpp"

namespace mapp {

class App;

class AppContext
{
public:
	void setApp(App* app);
	void setWindow(Window* window);

	App* getApp() { return mApp; }
	Window* getWindow() { return mWindow; }

private:
	App* mApp;
	Window* mWindow;
};

class App
{
public:
	explicit App(Window* window = nullptr);

	void run();
	void shutdown();
	void onEvent(Event& event);
	void pushLayer(Layer* layer);
	void pushOverlay(Layer* layer);

	float getDeltaTime() { return mDeltaTime; }

private:
	bool onWindowClose(WindowCloseEvent& event);
	bool onWindowResize(const WindowResizeEvent& event);

private:
	AppContext mContext;
	LayerStack mLayerStack;
	Window* mWindow;
	bool mIsRunning;
	float mLastFrameTime;
	float mDeltaTime;
};

}	// namespace mapp