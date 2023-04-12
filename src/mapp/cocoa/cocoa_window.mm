#include "../include/mapp/cocoa/cocoa_window.hpp"

#include <Cocoa/Cocoa.h>

namespace mapp
{
	WindowMac::WindowMac(const WindowParams& params)
		: Window(params)
	{
        
        NSApplication* application = [NSApplication sharedApplication];
               [application setActivationPolicy:NSApplicationActivationPolicyRegular];
        
               NSWindow* window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0,0,params.width,params.height)
                                   styleMask:NSWindowStyleMaskTitled |
                                   NSWindowStyleMaskClosable |
                                   NSWindowStyleMaskMiniaturizable |
                                   NSWindowStyleMaskResizable
                                   
                                                                backing:NSBackingStoreBuffered defer:NO];
               
               [window setTitle:@"My Window"];
               [window center];
               [window makeKeyAndOrderFront:nil];
               
               [application activateIgnoringOtherApps:YES];
               [application run];
	}

	WindowMac::~WindowMac()
	{
	}
	
	void WindowMac::onUpdate(const float& dt)
	{
	}

	void* WindowMac::getNativeWindow()
	{
		return nullptr;
	}

	void* WindowMac::getNativeDisplay()
	{
		return nullptr;
	}
}
