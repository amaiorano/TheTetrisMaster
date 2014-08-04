#ifndef _COLOR_H_
#define _COLOR_H_

// Forward declaration for typedefs
template <typename T, int MIN, int MAX> struct Color3;
template <typename T, int MIN, int MAX> struct Color4;

// Some very useful typedefs
typedef Color3<float, 0, 1> Color3F; // F = float
typedef Color3<unsigned char, 0, 255> Color3UB; // UB = unsigned byte
typedef Color4<float, 0, 1> Color4F; // F = float
typedef Color4<unsigned char, 0, 255> Color4UB; // UB = unsigned byte


// Represents an RGB color value
// T is the underlying color component data type,
// MIN and MAX define the minimum and maximum value for the color components.
// Note: template constant parameters must be integral types, so unfortunately,
// you cannot specify a value of say 1.5f for T=float.
template <typename T, int MIN=0, int MAX=1>
struct Color3
{
	T r, g, b;

	T Min() const { return static_cast<T>(MIN); }
	T Max() const { return static_cast<T>(MAX); }

	// Parameterless constructor - does not initialize members on purpose for efficiency
	Color3() { }

	// Constructor that intializes color components
	Color3(T red, T green, T blue) : r(red), g(green), b(blue) { }

	// Sets color components
	void Set(T red, T green, T blue)
	{
		r = red;  g = green;  b = blue;
	}

	// Conversion from other Color3 template types
	// i.e. Color3<float> cf(Color3<unsigned char>(0, 255, 255));
	template <typename T2, int MIN2, int MAX2>
	Color3(Color3<T2,MIN2,MAX2> const& color3)
	{
		r = static_cast<T>( color3.r * Max() / color3.Max() );
		g = static_cast<T>( color3.g * Max() / color3.Max() );
		b = static_cast<T>( color3.b * Max() / color3.Max() );
	}

	// Conversion from other Color3 template types
	// i.e. Color3<float> cf = Color3<unsigned char>(0, 255, 255);
	template <typename T2, int MIN2, int MAX2>
	void operator=(Color3<T2,MIN2,MAX2> const& color3)
	{
		// Convert this from input color
		//
		// r / Max() = color3.r / color3.Max()
		// so r = color3.r * Max() / color3.Max()
		//
		// similar for g and b

		r = static_cast<T>( color3.r * Max() / color3.Max() );
		g = static_cast<T>( color3.g * Max() / color3.Max() );
		b = static_cast<T>( color3.b * Max() / color3.Max() );
	}

	// Functions that return specific and often-used colors. Since they return const
	// refs, these can be used efficiently as is or assigned to Color3 objects, in
	// which case a copy will be made.	
	static const Color3& Black()	{ static Color3 color(MIN, MIN, MIN); return color; }
	static const Color3& White()	{ static Color3 color(MAX, MAX, MAX); return color; }
	static const Color3& Red()		{ static Color3 color(MAX, MIN, MIN); return color;  }
	static const Color3& Green()	{ static Color3 color(MIN, MAX, MIN); return color;  }
	static const Color3& Blue()		{ static Color3 color(MIN, MIN, MAX); return color;  }
	static const Color3& Magenta()	{ static Color3 color(MAX, MIN, MAX); return color;  }
	static const Color3& Teal()		{ static Color3 color(MIN, MAX, MAX); return color;  }
	static const Color3& Yellow()	{ static Color3 color(MAX, MAX, MIN); return color;  }
};

// Similar to Color3, but also contains the alpha component
template <typename T, int MIN=0, int MAX=1>
struct Color4
{
	T r, g, b, a;

	T Min() { return static_cast<T>(MIN); }
	T Max() { return static_cast<T>(MAX); }

	// Parameterless constructor - does not initialize members on purpose for efficiency
	Color4() { }

	// Constructor that intializes color components
	Color4(T red, T green, T blue, T alpha) : r(red), g(green), b(blue), a(alpha) { }

	// Constructor that initializes from a Color3 and an alpha value.
	// Note that since alpha has a default value, a Color3 can be casted implicitly
	// to a Color4 if desired.
	Color4(const Color3<T,MIN,MAX>& color3, T alpha=1) : r(color3.r), g(color3.g), b(color3.b), a(alpha) {}

	// Assigning a Color3 to a Color4 sets the RGB components (alpha is not modified)
	void operator=(Color3<T,MIN,MAX> const& color3)
	{
		r = color3.r;  g = color3.g;  b = color3.b;
	}	

	// Sets color components
	void Set(T red, T green, T blue, T alpha)
	{
		r = red;  g = green;  b = blue;  a = alpha;
	}

	// Conversion from other Color4 template types
	template <typename T2, int MIN2, int MAX2>
	void operator=(Color4<T2,MIN2,MAX2> const& color4)
	{
		r = static_cast<T>( color4.r * Max() / color4.Max() );
		g = static_cast<T>( color4.g * Max() / color4.Max() );
		b = static_cast<T>( color4.b * Max() / color4.Max() );
	}
};

#endif // _COLOR_H_
