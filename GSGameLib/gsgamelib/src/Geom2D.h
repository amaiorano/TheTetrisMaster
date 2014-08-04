#ifndef _GEOM_2D_H_
#define _GEOM_2D_H_

template <typename T>
class Point2d
{
public:
	T x, y;

	Point2d() : x(T()), y(T())			{}
	Point2d(T X, T Y) : x(X), y(Y)		{}

	// Explicit conversion constructor - must cast to T2
	template <typename T2>
	explicit Point2d(Point2d<T2> const& rhs)
		: x( static_cast<T>(rhs.x) ), y( static_cast<T>(rhs.y) )
	{
	}

	void Set(T X, T Y)					{ x = X; y = Y; }
	void Get(T& X, T& Y)				{ X = x; Y = y; }
	void Offset(T xOffset, T yOffset)	{ x += xOffset; y += yOffset; }
};


template <typename T>
class Size2d
{
public:
	T w, h;

	Size2d() : w(T()), h(T())			{}
	Size2d(T W, T H) : w(W), h(H)		{}

	// Explicit conversion constructor - must cast to T2:
	// Size2d<float> f;
	// Size2d<int> i = static_cast<Size2d<int> >(f); // cast is necessary
	template <typename T2>
	explicit Size2d(Size2d<T2> const& rhs)
		: w( static_cast<T>(rhs.w) ), h( static_cast<T>(rhs.h) )
	{
	}

	void Set(T W, T H)					{ w = W; h = H; }
	void Get(T& W, T& H)				{ H = h; W = w; }
};


// TODO: CODE THIS CLASS
template <typename T>
class Rect2d
{

};

#endif // _GEOM_2D_H_
