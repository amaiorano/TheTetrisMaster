#ifndef _COLOR_FUNCS_H_
#define _COLOR_FUNCS_H_

#include "Color.h"

namespace ColorFuncs
{
	// Extracts RGB components from 16-bit color value that 
	// has format: bbbb bggg gggr rrrr (5-6-5)	
	Color3UB ExtractRGB565(unsigned short color16);
}

#endif // _COLOR_FUNCS_H_
