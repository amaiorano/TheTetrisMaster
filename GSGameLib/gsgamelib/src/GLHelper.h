#ifndef _GL_HELPER_H_
#define _GL_HELPER_H_

#include <windows.h>		// Win32 main include
#include <gl/gl.h>			// OpenGL main include
#include <gl/glu.h>			// OpenGL utilities
#include "SmartAssert.h"

#ifdef _DEBUG
	#define ASSERT_NO_GL_ERROR() /*GLHelperInternal::*/AssertNoGLError();

	namespace //GLHelperInternal
	{
		void AssertNoGLError()
		{
			GLenum glError = glGetError();
			switch (glError)
			{
				case GL_NO_ERROR: break;
				case GL_INVALID_ENUM		: SMART_ASSERT(glError != GL_INVALID_ENUM).msg("GL Error: Invalid enum"); break;
				case GL_INVALID_VALUE		: SMART_ASSERT(glError != GL_INVALID_VALUE).msg("GL Error: Invalid value"); break;
				case GL_INVALID_OPERATION	: SMART_ASSERT(glError != GL_INVALID_OPERATION).msg("GL Error: Invalid operation"); break;
				case GL_STACK_OVERFLOW		: SMART_ASSERT(glError != GL_STACK_OVERFLOW).msg("GL Error: Stack overflow"); break;
				case GL_STACK_UNDERFLOW		: SMART_ASSERT(glError != GL_STACK_UNDERFLOW).msg("GL Error: Stack underflow"); break;
				case GL_OUT_OF_MEMORY		: SMART_ASSERT(glError != GL_OUT_OF_MEMORY).msg("GL Error: Out of memory"); break;
				default						: SMART_ASSERT(false)(glError).msg("GL Error: unknown code");
			}
		}
	}
#else
	#define ASSERT_NO_GL_ERROR() ((void)0)
#endif

class GLHelper
{
	public:
		static void SetGLParam(GLenum param, bool enable=true); // Can use instead of glEnable()/glDisable()
		static bool ToggleGLParam(GLenum param); // Toggles between enabled/disabled state of param, returns new state

		static void SetWireFrame(bool wireFrame=true); // Draw everything using wireframe or solid?
		static void SetShadeModel(bool smooth=true); // If smooth is false, "flat" shading will be used

		// Sets the drawing order of polygon front-faces (counter-clockwise or clockwise?) and whether
		// OpenGL should cull (not draw) backfaces
		static void SetFrontFace(bool ccw, bool cullBackFace);

		// Sets up depth buffer values: whether to use it, type of depth function, and buffer clearing value
		static void SetDepthBuffer(bool enable=true, GLenum depthFunc=GL_LEQUAL, GLclampd clearVal=1.0f);

		// This function sets up material color-tracking
		static void SetColorMaterial(bool enable=true, GLenum face=GL_FRONT, GLenum mode=GL_AMBIENT_AND_DIFFUSE);

		static void SetTexturing(bool enable=true);
		static void SetBlending(bool enable=true);

		// Lighting functions
		static void SetLighting(bool enable=true);
		static void SetAmbientLight(GLfloat r, GLfloat g, GLfloat b, GLfloat a=1.0f);

		// Creates a texture memory buffer and returns the unique integer id for it
		static int CreateTexture(GLint channels, GLint width, GLint height, GLvoid* pData);
		static void CreateTexture(GLuint& rTexId, GLint channels, GLint width, GLint height, GLvoid* pData);
};


inline void GLHelper::SetGLParam(GLenum param, bool enable/*=true*/)
{
	if (enable)
		glEnable(param);
	else
		glDisable(param);

	ASSERT_NO_GL_ERROR();
}

inline bool GLHelper::ToggleGLParam(GLenum param)
{
	bool newVal;
	switch (glIsEnabled(param))
	{
		case GL_TRUE:	
			glDisable(param);
			newVal = false;
			break;

		case GL_FALSE:
			glEnable(param);
			newVal = true;
			break;

		default:
			SMART_ASSERT(false).msg("Invalid GL parameter to toggle");
			break;
	}

	return newVal;
}

inline void GLHelper::SetWireFrame(bool wireFrame/*=true*/)
{
	glPolygonMode(GL_FRONT_AND_BACK, wireFrame? GL_LINE : GL_FILL);
	ASSERT_NO_GL_ERROR();
}

inline void GLHelper::SetShadeModel(bool smooth/*=true*/)
{
	glShadeModel(smooth? GL_SMOOTH : GL_FLAT);
	ASSERT_NO_GL_ERROR();
}

inline void GLHelper::SetFrontFace(bool ccw, bool cullBackFace)
{
	// Take inverted Y into consideration
	glFrontFace(ccw? GL_CCW : GL_CW); // Set front-face orientation
	
	// Enable/Disable culling and make sure to set it to back-face if enabled
	SetGLParam(GL_CULL_FACE, cullBackFace? GL_TRUE : GL_FALSE);
	
	if ( cullBackFace )
		glCullFace(GL_BACK);
	
	ASSERT_NO_GL_ERROR();
}

inline void GLHelper::SetDepthBuffer(bool enable/*=true*/, GLenum depthFunc/*=GL_LEQUAL*/, GLclampd clearVal/*=1.0f*/)
{
	glClearDepth(clearVal);	// Depth buffer clearing value
	glDepthFunc(depthFunc);	// The type of depth testing to do
	SetGLParam(GL_DEPTH_TEST, enable);
}

inline void GLHelper::SetColorMaterial(bool enable/*=true*/, GLenum face/*=GL_FRONT*/, GLenum mode/*=GL_AMBIENT_AND_DIFFUSE*/)
{
	glColorMaterial(face, mode);
	SetGLParam(GL_COLOR_MATERIAL, enable);
}

inline void GLHelper::SetTexturing(bool enable/*=true*/)
{
	SetGLParam(GL_TEXTURE_2D, enable);
}

inline void GLHelper::SetBlending(bool enable/*=true*/)
{
	SetGLParam(GL_BLEND, enable);
}

inline void GLHelper::SetLighting(bool enable/*=true*/)
{
	SetGLParam(GL_LIGHTING, enable);
}


inline void GLHelper::SetAmbientLight(GLfloat r, GLfloat g, GLfloat b, GLfloat a/*=1.0f*/)
{
	float arr[4] = {r, g, b, a};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, arr);
	ASSERT_NO_GL_ERROR();
}

inline /*static*/ int GLHelper::CreateTexture(GLint channels, GLint width, GLint height, GLvoid* pData)
{
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, 
		channels==3? GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, pData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	ASSERT_NO_GL_ERROR();

	return texId;
}

inline /*static*/ void GLHelper::CreateTexture(GLuint& rTexId, GLint channels, GLint width, GLint height, GLvoid* pData)
{
	rTexId = CreateTexture(channels, width, height, pData);
	ASSERT_NO_GL_ERROR();
}

#endif // _GL_HELPER_H_
