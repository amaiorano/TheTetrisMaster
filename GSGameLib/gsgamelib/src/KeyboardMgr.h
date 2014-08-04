#ifndef _KEYBOARD_MGR_H_
#define _KEYBOARD_MGR_H_

#include "Window.h"
#include "Singleton.h"

// For all platforms, we use the VKEY_ defines from Windows.
// These must be defined for non-Windows platforms.
#ifdef WIN32
#include <windows.h>
#include <winuser.h>
#else
#error Virtual Keys (VKEY_*) must be defined on this platform
#endif

const int NUM_KEYS = 256;
enum eKeyState { KEY_UP, KEY_DOWN };

// Represents the state of one key. KeyboardMgr (below) manages
// an array of KeyStates.
class KeyState
{
public:
	KeyState() : m_bLastDown(false), m_bCurrDown(false), m_fDownTimeMS(0) {}

	// Returns true if key is currently down
	bool Down()	const			{ return m_bCurrDown; }

	// This overload also returns true if key is currently down; however,
	// it also returns whether the key has been down at least the input amount
	// of time (fMinTimeMS) into the output parameter rbDownTimeElapsed. You
	// must supply the change in time since the last call into fDeltaTimeMS.
	// Q: Why not just return the output param value directly from the function?
	// A: So that you can ensure mutual exclusion between keys by using simple
	// if-elseif conditionals.
	bool Down(float fMinTimeMS, float fDeltaTimeMS, bool& rbDownTimeElapsed) const;

	// Returns true if key is currently up
	bool Up() const				{ return !m_bCurrDown; }

	// Returns the state (up or down)
	eKeyState GetState()const	{ return (m_bCurrDown? KEY_DOWN : KEY_UP); }

	// Returns true if the key was JUST pressed (true on first key down)
	bool Pressed() const
	{
		if ( !m_bLastDown && m_bCurrDown )
		{
			m_bLastDown = true; // Next time it won't be just pressed
			return true;
		}
		return false;
	}
	
	// Returns true if key was JUST released (true on first key up)
	bool Released()	const
	{ 
		if ( m_bLastDown && !m_bCurrDown )
		{
			m_bLastDown = false; // Next time it won't be just released
			return true;
		}
		return false;
	}

private: 
	// These private functions are called by KeyboardMgr
	friend class KeyboardMgr;

	void SetDown()
	{
		m_bLastDown = m_bCurrDown;
		m_bCurrDown = true;
	}

	void SetUp()
	{
		m_bLastDown = m_bCurrDown;
		m_bCurrDown = false;
	}

private:
	mutable bool m_bLastDown;		// True if key was down last iteration
	mutable bool m_bCurrDown;		// True if key is down this iteration
	
	mutable float m_fDownTimeMS;	// How much time it has been down
	mutable bool m_bLastDownInternal;
};


// Manages an array of KeyStates. The client queries the KeyboardMgr
// to get the state of specific keys.
class KeyboardMgr : public Singleton<KeyboardMgr>
{
protected:
	// Non-public constructor for singleton
	KeyboardMgr() : m_rWindow(Window::Instance())
	{
	}

public:
	const KeyState& GetKeyState(VKEY vKey) const
	{
		UpdateKeyState(vKey);
		return m_arrKeyStates[vKey];
	}

	// Same as GetKeyState() but more convenient
	const KeyState& operator[](VKEY vKey) const
	{
		UpdateKeyState(vKey);
		return m_arrKeyStates[vKey];
	}

	// Call to reset the internal state
	void SetAllKeysUp()
	{
		for (int i=0; i<NUM_KEYS; ++i)
		{
			m_arrKeyStates[i].SetUp();
			m_arrKeyStates[i].SetUp();
		}
	}

private:
	// Updates key state for input key
	void UpdateKeyState(VKEY vKey) const
	{
		if ( m_rWindow.PollKeyState(vKey) )
			m_arrKeyStates[vKey].SetDown();
		else
			m_arrKeyStates[vKey].SetUp();
	}

private:
	mutable KeyState m_arrKeyStates[NUM_KEYS];
	IWindow& m_rWindow;
};


#endif // _KEYBOARD_MGR_H_
