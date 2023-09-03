#pragma once

#include <vector>

namespace mapp {

class Layer
{
public:
	virtual void update(const float dt) = 0;
	virtual void postUpdate(const float dt) = 0;
};

using LayerList = std::vector<Layer*>;

}