#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include "IGameEngine.h"
#include "Singleton.h"
#include "Window.h"
#include "IViewport.h"
#include "KeyboardMgr.h"
#include "Renderer.h"
#include "FrameRateTimer.h"
#include "AudioStreamMgr.h"

// GameApp will handle and display any exceptions that may be thrown
// during load/unload. Note: exception handling is not used during the
// game loop for speed purposes.
#define GS_HANDLE_EXCEPTIONS

enum GameAppRetVal
{
	GAMEAPP_OK = 0,
	GAMEAPP_FAIL = -1,
	GAMEAPP_EXCEPTION = -2
};


// The main app object, implemented as a singleton
class GameApp : public Singleton<GameApp>, protected IEventListener
{
protected:
	GameApp(); // Make singleton
public:
	~GameApp();

	// Optional configuration functions
	void SetViewport(std::auto_ptr<IViewport> pViewport)	{ m_pViewport = pViewport; }

	// Call to run the app, pass in your own derived IGameEngine
	int Run(IGameEngine& rGameEngine);

	// Call to quit the app (called automatically when you close the window)
	void Quit(int iQuitStatus=GAMEAPP_OK);

	// Accessors
	IGameEngine& GetGameEngine()		{ return *m_pGameEngine; }
	IWindow& GetWindow()				{ return Window::Instance(); }
	IRenderer& GetRenderer()			{ return Renderer::Instance(); }
	AudioStreamMgr& GetAudioStreamMgr()	{ return AudioStreamMgr::Instance(); }

	// Returns change in time since last frame. Use for timing calculations.
	float GetDeltaTimeMS() const { return m_fDeltaTimeMS; }

private:
	virtual bool OnEvent(const Event& rEvent);
	
	// Attempts to handle the exception. Returns false if it fails, upon which
	// the exception should be rethrown.
	bool HandleException(std::string strContext, std::exception* pEx=NULL);

private:
	IGameEngine* m_pGameEngine;
	FrameRateTimer m_frameRaterTimer;
	std::auto_ptr<IViewport> m_pViewport;

	bool m_bQuit;
	int m_iQuitStatus;
	float m_fDeltaTimeMS;
};

#endif // _GAME_APP_H_
