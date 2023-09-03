#pragma once

#include "dialogs_desc.hpp"

namespace mapp
{

void showMessageBox(const MessageDesc& mdesc);

bool showOpenDialog(const OpenSaveDialogDesc& odesc, std::string& outPath);

bool showSaveDialog(const OpenSaveDialogDesc& sdesc, std::string& outPath);

}