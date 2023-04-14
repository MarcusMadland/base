#ifdef MAPP_PLATFORM_COCOA

#include "../include/mapp/cocoa/cocoa_window.hpp"

#include <Cocoa/Cocoa.h>

namespace mapp
{
	WindowCocoa::WindowCocoa(const WindowParams& params)
		: Window(params)
	{
        NSApplication* application = [NSApplication sharedApplication];
               [application setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        NSWindow* window = [[NSWindow alloc]            initWithContentRect:NSMakeRect(0,0,params.width,params.height)
                               styleMask:NSWindowStyleMaskTitled |
                               NSWindowStyleMaskMiniaturizable |
                               (params.canClose    ? NSWindowStyleMaskClosable : 0) |
                               (params.canResize   ? NSWindowStyleMaskResizable : 0)
                              
                                                        backing:NSBackingStoreBuffered defer:NO];
        
        NSString *finalTitle = [NSString stringWithCString:params.title.c_str()
                                           encoding:[NSString defaultCStringEncoding]];
               [window setTitle:finalTitle];
               [window center];
               [window makeKeyAndOrderFront:nil];
               
               [application activateIgnoringOtherApps:YES];
               [application run];
	}

	WindowCocoa::~WindowCocoa()
	{
	}
	
	void WindowCocoa::onUpdate(const float& dt)
	{
	}

	void* WindowCocoa::getNativeWindow()
	{
		return nullptr;
	}

	void* WindowCocoa::getNativeDisplay()
	{
		return nullptr;
	}
}

#endif
