#pragma once

#include "mapp/common/event.hpp"
#include "mapp/common/init.hpp"
#include <queue>

namespace mapp
{
struct MacEvent
{
};
/**
 * MacOS Events can be per window virtual functions or received from a queue.
 * Events - https://developer.apple.com/documentation/appkit/nsevent
 */
class EventQueue
{
  public:
    void update();

    const Event& front();

    void pop();

    bool empty();

  protected:
    void pushEvent(MacEvent me);

    std::queue<Event> mQueue;
};
}
