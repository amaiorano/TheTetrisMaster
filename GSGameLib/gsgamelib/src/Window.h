#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "IWindow.h"

// Container and factory for IWindow
class Window
{
public:
	static IWindow& Instance();

private:
	Window() {}
};


#endif // _WINDOW_H_
