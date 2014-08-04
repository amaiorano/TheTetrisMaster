#include "GameApp.h"

#include "string_helpers.h"
#include "SmartAssert.h"
#include "Persp2dViewport.h"
#include "Ortho2dViewport.h"
#include "GameClock.h"

using namespace std;

GameApp::GameApp() 
: m_bQuit(false), m_iQuitStatus(0)
{
}

GameApp::~GameApp()
{
}

int GameApp::Run(IGameEngine& rGameEngine)
{
	////////////////////
	// Initialization
	////////////////////

	// Variables declared outside of try scope because they are
	// used outside the try scope.
	string strTitle;
	IWindow& rWindow = Window::Instance();

#ifdef GS_HANDLE_EXCEPTIONS
	try
	{
#endif

	// Set assertion log
#ifdef _DEBUG
	SmartAssert::set_log("asserts.txt");
#else
	SmartAssert::set_log(cerr);
#endif

	// Set the one and only app client object
	m_pGameEngine = &rGameEngine;

	// Register for specific window events
	// (we can safely do this even before window is created)
	rWindow.AddEventListener(*this, WMT_ACTIVATE);

	// First allow client to perform any initialization required
	if ( !m_pGameEngine->Initialize() )
		return false;

	// Allow user to specify window creation params
	// (all variables are passed by reference as output parameters)
	strTitle = "Guardian Soft Game Library";
	int iWidth = 640;
	int iHeight = 480;
	int iBitsPerPixel = 16;
	bool bFullScreen = false;

	m_pGameEngine->PreCreateWindow(strTitle, iWidth, iHeight, iBitsPerPixel, bFullScreen);

	// Create the window
	rWindow.Create(strTitle, iWidth, iHeight, iBitsPerPixel, bFullScreen);

	// Create and bind the viewport to the window	
	if ( m_pViewport.get() == NULL )
	{
		//m_pViewport.reset( new Persp2dViewport(true) ); // Y-positive down!
		m_pViewport.reset( new Ortho2dViewport(true) );
	}
	m_pViewport->Bind(rWindow);

	// Now we can initialize the Renderer
	Renderer::Instance().Inititialize();

	// Display the window and immediately swap the backbuffer - this will clear
	// the screen/window to the current color buffer value
	rWindow.Show();
	m_pViewport->PreRenderFrame(); // Setup viewport (clears color buffer)
	m_pViewport->SwapBuffers(); // This will clear the screen to current color buffer value

	// Allow client to specify more graphics settings
	m_pGameEngine->InitializeRenderer(Renderer::Instance());

	// All client to intialize the audio device
	m_pGameEngine->InitializeAudioStreamMgr( AudioStreamMgr::Instance() );

	// Allow client to load up game data
	if ( !m_pGameEngine->LoadGameData() )
		return GAMEAPP_FAIL;

#ifdef GS_HANDLE_EXCEPTIONS
	} // end try
	catch (std::exception& ex)
	{
		if ( !HandleException("Loading", &ex) )
			throw; // Rethrow
		return GAMEAPP_EXCEPTION;
	}
	catch (...)
	{
		if ( !HandleException("Loading") )
			throw; // Rethrow
		return GAMEAPP_EXCEPTION;
	}
#endif // GS_HANDLE_EXCEPTIONS


	////////////////////
	// Main game loop
	////////////////////

	// NOTE: We don't use exception handling during the game loop because that
	// would compromise speed/space.

	KeyboardMgr& rKeyboardMgr = KeyboardMgr::Instance();
	
	m_bQuit = false;
	while (!m_bQuit)
	{
		// Let Window process messages, returns true if QUIT message is received
		if ( rWindow.ProcessMessages(m_bQuit) )
		{
			if ( m_bQuit )
				Quit(true); // Quit normally
		}
		else // No message was processed, so do game logic...
		{
			if ( rWindow.IsActive() ) // Skip game logic if window is inactive
			{
				// Must call at start of frame
				m_frameRaterTimer.FrameBegin();

				// Did user press escape?
				if ( rKeyboardMgr.GetKeyState(VK_ESCAPE).Pressed() )
				{
					Quit();
					continue; // Will break the loop
				}

				// Process input from user
				m_pGameEngine->ProcessInput(rKeyboardMgr);

				// Let client do pre-rendering stuff
				m_pGameEngine->PreFrameLogic();

				// Before rendering the scene, setup the viewport
				m_pViewport->PreRenderFrame();

				// Let client render the frame
				bool bSwapDrawBuffers = m_pGameEngine->RenderFrame(Renderer::Instance());

				// If RenderFrame() returned true, then swap the buffers
				if (bSwapDrawBuffers)
					m_pViewport->SwapBuffers();

				// Let client do post-rendering stuff
				m_pGameEngine->PostFrameLogic();

				// Retrieve time between current and last frame
				m_fDeltaTimeMS = m_frameRaterTimer.GetFrameIntervalMS();

				// Must call at end of frame
				m_frameRaterTimer.FrameEnd();

				// Show FPS in window title				
				rWindow.SetTitle( str_format("%s (FPS: %d)", strTitle.c_str(), m_frameRaterTimer.GetFPS()) );

				// Update game clock once per frame
				GameClock::Instance().Update();
			}

			// Always let system breathe (avoids 100% CPU consumption)
			rWindow.Sleep(1);		
		} // End game logic branch
	} // End while (!m_bQuit)

	////////////////////
	// Clean up
	////////////////////

#ifdef GS_HANDLE_EXCEPTIONS
	try
	{
#endif

	m_pViewport->Unbind(rWindow);
	rWindow.Destroy();

	// Unload the client's game data
	if ( !m_pGameEngine->UnloadGameData() )
		return GAMEAPP_FAIL;

#ifdef GS_HANDLE_EXCEPTIONS
	} // end try
	catch (std::exception& ex)
	{
		if ( !HandleException("Unloading", &ex) )
			throw; // Rethrow
		return GAMEAPP_EXCEPTION;
	}
	catch (...)
	{
		if ( !HandleException("Unloading") )
			throw; // Rethrow
		return GAMEAPP_EXCEPTION;
	}
#endif // GS_HANDLE_EXCEPTIONS

	// Return final quit status
	return m_iQuitStatus;
}

void GameApp::Quit(int iQuitStatus/*=GAMEAPP_OK*/)
{
	m_bQuit = true;
	m_iQuitStatus = iQuitStatus;
}

bool GameApp::OnEvent(const Event& rEvent)
{
	switch (rEvent.type)
	{
	case WMT_ACTIVATE:
		{
			CAST_EVENT(rActivateEvent, ActivateWindowEvent);

			// Pause/Resume all audio based on window activation state
			if ( rActivateEvent.bActivated )
				GetAudioStreamMgr().ResumeAll();
			else
				GetAudioStreamMgr().PauseAll();

			return true;
		}
	} // end switch (rEvent.type)

	return false;
}

bool GameApp::HandleException(std::string strContext, std::exception* pEx/*=NULL*/)
{
	IWindow& rWindow = Window::Instance();

	try // Try in case this code throws
	{
		if ( rWindow.IsFullScreen() )
			rWindow.RestoreScreenMode();

		string strCaption = rWindow.GetTitle() + " - Exception Caught (" + strContext + ")";
		string strMsg = (pEx != NULL)? pEx->what() : "Unknown exception was caught";

		rWindow.MsgBoxOk(ERROR_ICON, strCaption, strMsg);
	}
	catch (...)
	{
		return false; // Failed to handle this exception
	}
	
	return true; // Exception handled
}
