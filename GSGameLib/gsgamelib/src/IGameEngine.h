#ifndef _IGAME_ENGINE_H_
#define _IGAME_ENGINE_H_

#include <string>
#include "AudioStreamMgr.h"

// Forward declarations
class GameApp; 
class IRenderer;
class KeyboardMgr;

// You should derive your own IGameEngine object and implement
// the virtual member functions in it. Pass this object to
// GameApp::Run() to have the GameApp instance use it.
class IGameEngine
{
protected:
	friend class GameApp; // GameApp has full access to this class

	// Very first function to be called by the GameApp, so override to
	// check that everything is in order (file existance, etc.) and to
	// perform one-time initialization routines. Do not load your game
	// data here, use LoadGameData() (below) instead.
	virtual bool Initialize()
	{
		return true;
	}

	// Called just before Window instance is created.
	// Override to specify your own values for the window creation
	virtual void PreCreateWindow(std::string& strTitle, int& iWidth, int& iHeight, int& iBitsPerPixel, bool& bFullScreen)
	{
	}

	// Override to specify/override initial renderer setup
	virtual void InitializeRenderer(IRenderer& rRenderer)
	{
	}

	// Override to specify/override initial audio player setup
	// By default, it initializes the audio device.
	virtual void InitializeAudioStreamMgr(AudioStreamMgr& rAudioStreamMgr)
	{
		rAudioStreamMgr.Initialize();
	}

	// Override to load all your game data. If your game loads different
	// data depending on current state, you should check the state and load
	// the relevant data here. Return false if loading fails, true otherwise.
	virtual bool LoadGameData()
	{
		return true;
	}

	// Override to unload the data loaded in LoadGameData() - this is
	// necessary for switching between full-screen and windowed mode, and
	// is called when exiting. You can always call this function yourself to
	// unload current game  data based on state, change the state, then call
	// LoadGameData() afterwards. Return false if unloading fails, true
	// otherwise.
	virtual bool UnloadGameData()
	{
		return true;
	}


	///////////////////////////////////////////////////////////////////////
	// GAME LOOP FUNCTIONS : These are called repeatedly and in the order
	// they are presented below until the user quits.

	// Override and put your code to process game input here. The keyboard
	// manager contains the current state of keys.
	virtual void ProcessInput(const KeyboardMgr& rKeyboardMgr)
	{
	}

	// Override to perform any logic just before the frame is about to be
	// rendered.
	virtual void PreFrameLogic()
	{
	}

	// Override and put your code to draw the scene here.
	// The return value specifies whether to go ahead and blit the frame.
	virtual bool RenderFrame(IRenderer& renderer)
	{
		return true;
	}

	// Called after RenderFrame(), regardless of whether screen was updated
	// or not.
	virtual void PostFrameLogic()
	{
	}

	// END GAME LOOP FUNCTIONS
	///////////////////////////////////////////////////////////////////////
};

#endif // _IGAME_ENGINE_H_
