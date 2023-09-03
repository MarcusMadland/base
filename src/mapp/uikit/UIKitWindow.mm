#include "mapp/uikit/UIKitWindow.h"

#import <UIKit/UIKit.h>
#import <MetalKit/MetalKit.h>

@interface XWinWindow : UIWindow
{
}
@end

@implementation XWinWindow

@end

@interface XWinViewController : UIViewController

@end

@interface XWinView : MTKView

@property mapp::Window* mappWindow;

@end

@implementation XWinView

- (void)setFrame:(CGRect)frame
{
	[super setFrame:frame];
	if( _mappWindow)
	{
		if( _mappWindow->mEventQueue)
		{
			float s = [[UIScreen mainScreen] scale];
			mapp::Event e = mapp::Event(mapp::ResizeData(s * frame.size.width, s * frame.size.height, false), _mappWindow);
			_mappWindow->mEventQueue->pushEvent(e);
		}
	}
}

@end

@implementation XWinViewController
{}

- (BOOL)prefersHomeIndicatorAutoHidden
{
	return YES;
}

- (BOOL)prefersStatusBarHidden
{
	return YES;
}

 -(UIStatusBarAnimation)preferredStatusBarUpdateAnimation
{
	return UIStatusBarAnimationSlide;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self
                                                                                 action:@selector(gestureDidRecognize:)];
    [self.view addGestureRecognizer:panGesture];
}

- (void)gestureDidRecognize:(UIGestureRecognizer *)gestureRecognizer
{
    UIPanGestureRecognizer *panGestureRecognizer = (UIPanGestureRecognizer *)gestureRecognizer;
    CGPoint velocity = [panGestureRecognizer velocityInView:self.view];
}

@end

namespace mapp
{


Window::Window()
{
	window =
	view =
	layer =
	viewController =
	mEventQueue = nullptr;
}

Window::~Window()
{
	if( window != nullptr)
	{
		close();
	}
}

bool Window::create(const WindowDesc& desc, EventQueue& eventQueue)
{
	mDesc = desc;
	UIApplication* app = (UIApplication*)getXWinState().application;

	CGRect rect = [[UIScreen mainScreen] bounds];
	CGRect nativeRect = [[UIScreen mainScreen] nativeBounds];
	float scale = [[UIScreen mainScreen] scale];
	
	if( !desc.fullscreen)
	{
		rect.size.width = desc.width;
		rect.size.height = desc.height;
	}
	mDesc.width = (unsigned)rect.size.width;
	mDesc.height = (unsigned)rect.size.height;

	
	// Configure view
	view = [[XWinView alloc] initWithFrame:rect];
	XWinView* v = (XWinView*)view;
	[v resignFirstResponder];
	[v setNeedsDisplay];
	[v setHidden:NO];
	[v setOpaque:YES];
	[v setAutoResizeDrawable:YES];
	v.backgroundColor = [UIColor clearColor];
	v.mappWindow = this;
	
	
	viewController = [[XWinViewController alloc] init];
	XWinViewController* vc = (XWinViewController*)viewController;
	[vc setView:v];
	[vc setNeedsUpdateOfHomeIndicatorAutoHidden];
	[vc setNeedsStatusBarAppearanceUpdate];
	
	// Configure window
	window = [[XWinWindow alloc] initWithFrame:rect];
	XWinWindow* w = (XWinWindow*)window;
	[w setRootViewController:vc];
	[w setContentMode:UIViewContentModeScaleToFill];
	[w makeKeyAndVisible];
	[w setBounds:rect];

	w.backgroundColor = [UIColor clearColor];
	
	float s = [[UIScreen mainScreen] scale];
	mapp::Event e = mapp::Event(mapp::ResizeData(s * rect.size.width, s * rect.size.height, false), this);
	eventQueue.pushEvent(e);
	eventQueue.update();
	
	mEventQueue = &eventQueue;

	return true;
}

WindowDesc Window::getDesc()
{
	return mDesc;
}

bool Window::update()
{
	return false;
}

void Window::close()
{
	[(XWinWindow*)window release];
	[(MTKView*)view release];
	[(XWinViewController*)viewController release];
	
	window = nullptr;
	view = nullptr;
	layer = nullptr;
}

void Window::setLayer(LayerType type)
{
	if(type == LayerType::Metal)
	{
		MTKView* v = (MTKView*)view;
		[v.layer setHidden:NO];
		[v.layer setOpaque:YES];
		[v.layer setNeedsDisplay];
		v.backgroundColor = [UIColor clearColor];
		layer = v.layer;
	}
	else if(type == LayerType::OpenGL)
	{
		//OpenGL is currently depreciated, and unavailable in UIKit
	}
}

void* Window::getWindowHandle()
{
    //return window;
    
    //XWinWindow* w = (XWinWindow*)window;
   // return (__bridge void*)w.layer;
    
    XWinView* v = (XWinView*)view;
    return (__bridge void*)v.layer;
}

}
