#include "vector.h"

Vector2d::operator Vector2dMA()
{ 
	return Vector2dMA(Mag(), Phi());
}

Vector2dMA Vector2d::ToVector2dMA()
{ 
	return Vector2dMA(Mag(), Phi());
}
