#include "ColorFuncs.h"

#include "ImageFuncs.h"

Color3UB ColorFuncs::ExtractRGB565(unsigned short color16)
{
	Color3UB color;
	color.r = static_cast<UBYTE>( (((color16 >> 11) & 0x1F) * 0xFF / 0x1F) );
	color.g = static_cast<UBYTE>( (((color16 >>  5) & 0x3F) * 0xFF / 0x3F)  );
	color.b = static_cast<UBYTE>( ((color16 & 0x1F) * 0xFF / 0x1F) );
	return color;
}

