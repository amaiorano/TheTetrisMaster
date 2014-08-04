#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "IRenderer.h"

// Container and factory for IRenderer
class Renderer
{
public:
	static IRenderer& Instance();

private:
	Renderer() {}
};

#endif // _RENDERER_H_
