#include "mapp/common/init.hpp"
#include "mapp/common/state.hpp"

namespace mapp
{
namespace
{
XWinState xWinState;
}

bool init(MainArgs)
{
    xWinState = XWinState(MainArgsVars);
    return true;
}
const mapp::XWinState& getXWinState() { return xWinState; }
}
