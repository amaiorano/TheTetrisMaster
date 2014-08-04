#ifndef _GL_RENDERER_H_
#define _GL_RENDERER_H_

#include "IRenderer.h"
#include "SmartAssert.h"

// Stupid Microsoft forces you to include Windows.h to use OpenGL
#ifdef WIN32
#include <windows.h>
#endif

// Include OpenGL-specific headers
#include <gl/gl.h>
#include <gl/glu.h>

#ifdef WIN32
// Link with libs
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "glaux.lib")
#endif

#define SET_GL_PARAM(bEnable, flag) (bEnable? glEnable(flag) : glDisable(flag))

// OpenGL graphics port
class GLRenderer : public IRenderer
{
public:
	GLRenderer() {}
	virtual ~GLRenderer() {}

	virtual void ResizeViewport(int x, int y, int iWidth, int iHeight)
	{
		glViewport(x, y, iWidth, iHeight);
	}

	virtual void Inititialize();

	virtual void SetColor(const Color4F& color)
	{
		glColor4f(color.r, color.g, color.b, color.a);
	}
	
	virtual Color4F GetColor()
	{
		GLfloat colors[4];
		glGetFloatv(GL_CURRENT_COLOR, (GLfloat*)&colors);
		return Color4F(colors[0], colors[1], colors[2], colors[3]);
	}

	virtual void ClearColorBuffer(const Color4F& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual void ClearDepthBuffer(float fDepth)
	{
		glClearDepth(fDepth);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

protected:
	virtual void DoMatrixMode(eMatrixMode mode);
public:

	virtual void LoadIdentity()
	{
		glLoadIdentity();
	}

	virtual void PushMatrix()
	{
		glPushMatrix();
	}

	virtual void PopMatrix()
	{
		glPopMatrix();
	}

	virtual void Translate(float x, float y, float z)
	{
		glTranslatef(x, y, z);
	}

	virtual void Translate(float x, float y)
	{
		glTranslatef(x, y, 0);
	}

	virtual void Rotate(const Angle& rAngle, float x, float y, float z)
	{
		glRotatef(rAngle, x, y, z);
	}

	virtual void Rotate(const Angle& rAngle)
	{
		glRotatef(rAngle, 0, 0, 1);
	}

	virtual void Scale(float x, float y)
	{
		glScalef(x, y, 0);
	}

	virtual void Orthographic(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar)
	{
		glOrtho(fLeft, fRight, fBottom, fTop, fNear, fFar);
	}

	virtual void Perspective(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar)
	{
		glFrustum(fLeft, fRight, fBottom, fTop, fNear, fFar);
	}

	virtual void Perspective(float fFieldOfViewAngle, float fAspectRatio, float fNear, float fFar)
	{
		gluPerspective(fFieldOfViewAngle, fAspectRatio, fNear, fFar);
	}

	virtual void SetTexturing(bool bEnable)
	{
		SET_GL_PARAM(bEnable, GL_TEXTURE_2D);
	}

	virtual TextureId LoadTexture(const ImageData& rImageData, Size2d<float>& rTextureSize);

protected:
	virtual void DoFreeTexture(TextureId& rTexId)
	{
		unsigned int tex = rTexId;
		glDeleteTextures(1, &tex);
	}
public:

	virtual void SelectTexture(const TextureId& rTexId, bool bSelect=true)
	{
		glBindTexture(GL_TEXTURE_2D, rTexId);
	}

	virtual void BeginRender(eShapeMode mode);

	virtual void EndRender()
	{
		glEnd();
	}

	virtual void Vertex(float x, float y)
	{
		glVertex2f(x, y);
	}

	virtual void TexCoord(float s, float t)
	{
		glTexCoord2f(s, t);
	}

	virtual void RenderXYZAxes(float fAxisSize=1.0f, float fAxisWidth=1.0f)
	{
		float currColor[4];
		glGetFloatv(GL_CURRENT_COLOR, currColor); // Save current color

		float fCurrLineWidth;
		glGetFloatv(GL_LINE_WIDTH, &fCurrLineWidth); // Save current line width

		glDisable(GL_TEXTURE_2D); // Disable texturing for line colors to show
		glLineWidth(fAxisWidth); // Set line width

		glBegin(GL_LINES);		
			// x-axis
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(fAxisSize, 0.0f, 0.0f);

			// y-axis
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, fAxisSize, 0.0f);

			// z-axis
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, fAxisSize);
		glEnd();
		
		glColor4fv(currColor); // Restore color
		glLineWidth(fCurrLineWidth); // Restore line width
		glEnable(GL_TEXTURE_2D); // Re-enable texturing
	}

	virtual void SetAlphaBlending(bool bEnable)
	{
		if ( bEnable )
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}
};


#endif // _GL_RENDERER_H_
