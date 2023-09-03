#pragma once

#include "mapp/common/event.hpp"
#include "mapp/common/init.hpp"
#include <queue>

namespace mapp
{

class EventQueue
{
public:
	void update();
	
	const Event &front();
	
	void pop();
	
	bool empty();
	
	void pushEvent(Event e);
	
	std::queue<Event> mQueue;
};
}
