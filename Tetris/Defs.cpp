// Defs.cpp

#include "Defs.h"
#include "Tetris.h"

TheTetrisMaster* GetGameEngine()
{
	return static_cast<TheTetrisMaster*>(&(GameApp::Instance().GetGameEngine()));
}

int Power(int x, int y)
{
	ASSERT(y>0, "y must be positive");

	int retVal = x;

	for (int i=1; i<y; ++i)
		retVal *= x;

	return retVal;
}

#ifdef _DEBUG
	char* GetFileFromPath(char* pFullPath)
	{
		size_t i = 0;
		for (i=strlen(pFullPath)-1; i>=0; --i)
			if (pFullPath[i] == '\\')
				break;
		return pFullPath+i+1;
	}
#endif
