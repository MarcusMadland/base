#include "../include/mapp/platform.hpp"

#ifdef MAPP_PLATFORM_COCOA

#include "../include/mapp/cocoa/cocoa_window.hpp"
#include "../include/mapp/app.hpp"

//#include <stdint.h>

#import <Cocoa/Cocoa.h>
//#import <CoreGraphics/CoreGraphics.h>

void* window = nullptr;
void* view = nullptr;
void* app = nullptr;
bool isFullscreen;

@interface nativeWindow : NSWindow
{
}
@end

@implementation nativeWindow
- (void) close
{
    if (mapp::App::isValid())
    {
        mapp::WindowCloseEvent event;
        mapp::App::getInstance().getWindow()->eventCallback(event);
    }
}
@end

@interface nativeView : NSView
- (BOOL)    acceptsFirstResponder;
- (BOOL)    isOpaque;
@end

@implementation nativeView

- (void) viewDidEndLiveResize
{
    [super viewDidEndLiveResize];
    
    if (mapp::App::isValid())
    {
        NSWindow* w = (NSWindow*)window;
        NSSize myNSWindowSize = [ [ w contentView ] frame ].size;
        
        mapp::WindowResizeEvent event = mapp::WindowResizeEvent(myNSWindowSize.width, myNSWindowSize.height);
        mapp::App::getInstance().getWindow()->eventCallback(event);
    }
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)isOpaque
{
    return YES;
}

@end

namespace mapp
{
	WindowCocoa::WindowCocoa(const WindowParams& params)
		: Window(params)
	{
        NSLog(@"Init cocoa window");
        
        // Application
        app = [NSApplication sharedApplication];
        NSApplication* nsApp = (NSApplication*)app;
            
        // Window
        NSRect rect = NSMakeRect(0, 0, params.width, params.height);
        NSWindowStyleMask styleMask = NSWindowStyleMaskTitled;
        if (params.canClose)
        {
            styleMask |= NSWindowStyleMaskClosable;
        }
        if (params.canResize)
        {
            styleMask |= NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;
        }
        if (!params.showBorder)
        {
            styleMask |= NSWindowStyleMaskFullSizeContentView | NSWindowStyleMaskBorderless;
        }
        
        window = [[nativeWindow alloc]
                  initWithContentRect: rect
                  styleMask: styleMask
                  backing: NSBackingStoreBuffered
                  defer: NO];
        
        void* title = [NSString stringWithCString:params.title.c_str()
                                    encoding:[NSString defaultCStringEncoding]];
        
        nativeWindow* w = ((nativeWindow*)window);
        NSWindowCollectionBehavior behavior =
                        NSWindowCollectionBehaviorFullScreenPrimary |
                        NSWindowCollectionBehaviorManaged;
        [w setCollectionBehavior:behavior];
        if(!params.title.empty())
        {
            [w setTitle: (NSString*)title];
            
        }
        [w center];
        [w setHasShadow:true];
        [w setTitlebarAppearsTransparent:!params.showBorder];

        // View
        rect = [w backingAlignedRect:rect options:NSAlignAllEdgesOutward];
        view = [[nativeView alloc] initWithFrame:rect];
        nativeView* v = (nativeView*)view;
        [v setHidden:NO];
        [v setNeedsDisplay:YES];
        [v setWantsLayer:YES];

        [w setContentView:(nativeView*)view];
        [w makeKeyAndOrderFront:nsApp];
        
        // Event
        
        @autoreleasepool
        {
            NSEvent* nsEvent = nil;
            do
            {
                nsEvent = [nsApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
                
                switch(nsEvent.type)
                {
                    case NSEventTypeSystemDefined:
                        
                        break;
                    case NSEventTypeKeyDown:
                    
                        break;
                    case NSEventTypeKeyUp:
                        
                        break;
                    case NSEventTypeLeftMouseDown:
                        
                        break;
                    case NSEventTypeLeftMouseUp:
                        
                        break;
                    case NSEventTypeRightMouseDown:
                        
                        break;
                    case NSEventTypeRightMouseUp:
                        
                        break;
                    case NSEventTypeMouseMoved:
                        
                        break;
                    case NSEventTypeScrollWheel:
                        
                        break;
                        
                    default:
                        
                        break;
                }
                
                [NSApp sendEvent:nsEvent];
            }
            while (nsEvent);
            
        }
        [nsApp updateWindows];
	}

	WindowCocoa::~WindowCocoa()
	{
        [(nativeWindow*)window release];
        [(nativeView*)view release];
        
        window = nullptr;
        view = nullptr;
	}
	
	void WindowCocoa::onUpdate(const float& dt)
	{
        NSEvent* nsEvent = [NSApp nextEventMatchingMask:NSEventMaskAny
                                   untilDate:[NSDate distantFuture]
                                      inMode:NSDefaultRunLoopMode
                                     dequeue:YES];
        
        [NSApp sendEvent:nsEvent];
	}

    bool WindowCocoa::setFullscreen(const bool enable)
    {
        if(enable && !isFullscreen)
        {
            isFullscreen = true;
            NSWindow* w = (NSWindow*)window;
            [w toggleFullScreen:nil];
            return true;
        }
        else if (!enable && isFullscreen)
        {
            isFullscreen = false;
            NSWindow* w = (NSWindow*)window;
            [w toggleFullScreen:nil];
            return true;
        }
        
        return false;
    }

    bool WindowCocoa::getIsFullscreen()
    {
        return isFullscreen;
    }

	void* WindowCocoa::getNativeWindow()
	{
		return window;
	}

	void* WindowCocoa::getNativeDisplay()
	{
		return nullptr;
	}
}

#endif
