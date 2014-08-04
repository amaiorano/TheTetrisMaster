///////////////////////////////////////////////////////////////////////////////
// Author:			Antonio Maiorano
// Creation Date:	December 16, 2003
// Last Updated:	December 16, 2003
//
// Description: A generic event notifier/listener system.
// Uses a form of the Observer pattern (GOF '94)
///////////////////////////////////////////////////////////////////////////////

#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include <map>
#include <vector>

// Event types will be of type int so that enums can be
// used to make life easier
typedef int tEventType;

// Derive your own Event classes/structs and provide
// unique type values for each class
struct Event
{
	Event(tEventType in_type) : type(in_type) {}	
	tEventType type;
};


class EventNotifier; // Forward declaration for friendship

// Interface for listener classes. Implement this interface to
// receive events. The implementor must add itself to the
// EventNotifier
class IEventListener
{
	friend class EventNotifier; // Calls OnEvent()

protected:
	// Implement to receive user-defined events.
	// Note: see CAST_EVENT() macro below.
	virtual bool OnEvent(const Event& rEvent) = 0;
};


// Mixin class for a notifier. Derive publicly to become a source
// of event notification.
class EventNotifier
{
public:
	// IEventListeners should add themselves or be added through this function
	void AddEventListener(IEventListener& rListener, tEventType type)
	{
		m_listenerTable[type].push_back(&rListener);
	}

protected:
	// Child class calls this function to notify registered listeners
	// that a given event has occured.
	// Returns false if no listeners are registered for this event type,
	// or if at least one listener returns false.
	bool NotifyListeners(const Event& rEvent)
	{
		// Grab reference to the list for input event type
		ListenerPtrList& rList = m_listenerTable[rEvent.type];

		if ( rList.empty() )
			return false; // No one to handle this event

		// Will become false if at least one listener returns false
		bool bHandled = true;

		// Iterate through the the list and dispatch the event
		ListenerPtrList::iterator it;
		for (it=rList.begin(); it != rList.end(); ++it)
		{
			IEventListener* pListener = *it;
			bHandled = bHandled && pListener->OnEvent(rEvent);
		}

		return bHandled;
	}

private:
	// Internally, we use a map of unique event types to vectors of listeners
	typedef std::vector<IEventListener*> ListenerPtrList;
	typedef std::map<tEventType, ListenerPtrList> TypeToListenerPtrListMap;
	TypeToListenerPtrListMap m_listenerTable;
};


// Macros...

// Useful macro that can be used within IEventListener::OnEvent() to
// cast the input Event& to your own type.
// Note: The Event& parameter must be named rEvent for this macro to work.
// Ex:
//
//	Assume struct ResizeWindowEvent : public Event has been defined...
//
//	virtual bool OnEvent(const Event& rEvent)
//	{
//		switch (rEvent.type)
//		{
//		case WMT_RESIZE:
//			{
//				CAST_EVENT(rEvent, ResizeWindowEvent);
//				std::cout << "\tResize: " << rEvent.iNewWidth << " x " << rEvent.iNewHeight << std::endl;
//			}
//			break;
//
//			...
//		}
//		return true;
//	}
//
#define CAST_EVENT(rVarName, EventType) \
	const EventType& rVarName = static_cast<const EventType&>(rEvent)

#endif // _EVENT_SYSTEM_H_
