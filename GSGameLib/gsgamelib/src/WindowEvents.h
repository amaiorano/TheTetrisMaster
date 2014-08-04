#ifndef _WINDOW_EVENTS_H_
#define _WINDOW_EVENTS_H_

#include "EventSystem.h"

// Contains Event implementations for Window messages.
// See Window::AddEventListener()

enum eWindowEventType
{
	WMT_RESIZE,
	WMT_MOVE,
	WMT_ACTIVATE,
	WMT_CLOSE,
	
};

struct ResizeWindowEvent : public Event
{
	ResizeWindowEvent(int in_iWidth, int in_iHeight) 
		: Event(WMT_RESIZE), iWidth(in_iWidth), iHeight(in_iHeight) {}

	int iWidth;
	int iHeight;
};

struct MoveWindowEvent : public Event
{
	MoveWindowEvent(int in_x, int in_y)
		: Event(WMT_MOVE), x(in_x), y(in_y) {}

	int x;
	int y;
};

struct ActivateWindowEvent : public Event
{
	ActivateWindowEvent(bool in_bActivated)
		: Event(WMT_ACTIVATE), bActivated(in_bActivated) {}

	bool bActivated;
};

struct CloseWindowEvent : public Event
{
	CloseWindowEvent()
		: Event(WMT_CLOSE) {}
};

#endif // _WINDOW_EVENTS_H_
