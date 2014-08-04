#ifndef _GSGAMELIB_H_
#define _GSGAMELIB_H_

///////////////////////////////////////////////////////////////////////////////
//                      GuardianSoft Game Library
//
// Author:      Antonio Maiorano
// E-Mail:      daroou@videotron.ca
//
// Description: This is the main header file for the GSGameLib, a C++ library
// that (hopefully) eases game development.
//
///////////////////////////////////////////////////////////////////////////////

// GameApp
#include "GameApp.h"
#include "IGameEngine.h"

// Graphics
#include "Texture.h"
#include "Renderer.h"
#include "IViewport.h"
#include "Ortho2dViewport.h"
#include "Persp2dViewport.h"
#include "Persp3dViewport.h"
#include "ImageData.h"
#include "Color.h"
#include "ColorFuncs.h"

// Window
#include "Window.h"
#include "WindowEvents.h"

// Input
#include "KeyboardMgr.h"

// Audio
#include "AudioStreamMgr.h"
#include "AudioStream.h"

// Math
#include "Geom2D.h"
#include "MathDefs.h"
#include "Vector.h"

// Utils
#include "string_helpers.h"
#include "HighResTimer.h"
#include "Config.h"

// GameCore
#include "GameClock.h"


// Under Windows, link directly with the lib. Make sure the
// lib path is set in Visual Studio.
#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "GSGameLib_debug.lib")
#else
#pragma comment(lib, "GSGameLib.lib")
#endif // _DEBUG
#endif // WIN32

#endif // _GSGAMELIB_H_
