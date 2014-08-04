#include "Renderer.h"

// NOTE:
// This is the only file that should ever include the IRenderer implementations
// since the factory function Instance() is defined here

#include "GLRenderer.h" // OpenGL implementation


IRenderer& Renderer::Instance()
{
	// Currently, only implementation is OpenGL
	static GLRenderer theRenderer;
	return theRenderer;
}