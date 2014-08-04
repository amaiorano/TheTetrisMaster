#ifndef _AUTO_ARRAY_H_
#define _AUTO_ARRAY_H_

// auto_array<> is equivalent to std::auto_array<> except that
// it manages a pointer to an array dynamically allocated
// using new[] and destroys it using delete[].

// Copied from std::auto_ptr by Microsoft and modified accordingly.
// See <memory> for std::auto_ptr implementation.

template<class _Ty>
	class auto_array;

template<class _Ty>
	struct auto_array_ref
		{	// proxy reference for auto_array copying
	auto_array_ref(auto_array<_Ty>& _Right)
		: _Ref(_Right)
		{	// construct from compatible auto_array
		}

	auto_array<_Ty>& _Ref;	// reference to constructor argument
	};

template<class _Ty>
	class auto_array
		{	// wrap an object pointer to ensure destruction
public:
	typedef _Ty element_type;

	explicit auto_array(_Ty *_Ptr = 0) _THROW0()
		: _Myptr(_Ptr)
		{	// construct from object pointer
		}

	auto_array(auto_array<_Ty>& _Right) _THROW0()
		: _Myptr(_Right.release())
		{	// construct by assuming pointer from _Right auto_array
		}

	auto_array(auto_array_ref<_Ty> _Right) _THROW0()
		: _Myptr(_Right._Ref.release())
		{	// construct by assuming pointer from _Right auto_array_ref
		}

	template<class _Other>
		operator auto_array<_Other>() _THROW0()
		{	// convert to compatible auto_array
		return (auto_array<_Other>(*this));
		}

	template<class _Other>
		operator auto_array_ref<_Other>() _THROW0()
		{	// convert to compatible auto_array_ref
		return (auto_array_ref<_Other>(*this));
		}

	template<class _Other>
		auto_array<_Ty>& operator=(auto_array<_Other>& _Right) _THROW0()
		{	// assign compatible _Right (assume pointer)
		reset(_Right.release());
		return (*this);
		}

	template<class _Other>
		auto_array(auto_array<_Other>& _Right) _THROW0()
		: _Myptr(_Right.release())
		{	// construct by assuming pointer from _Right
		}

	auto_array<_Ty>& operator=(auto_array<_Ty>& _Right) _THROW0()
		{	// assign compatible _Right (assume pointer)
		reset(_Right.release());
		return (*this);
		}

	auto_array<_Ty>& operator=(auto_array_ref<_Ty>& _Right) _THROW0()
		{	// assign compatible _Right._Ref (assume pointer)
		reset(_Right._Ref.release());
		return (*this);
		}

	~auto_array()
		{	// destroy the object
			// *** DIFFERENT FROM auto_ptr ***
		delete [] _Myptr;
		}
/*
	_Ty& operator*() const _THROW0()
		{	// return designated value
		return (*_Myptr);
		}

	_Ty *operator->() const _THROW0()
		{	// return pointer to class object
		return (&**this);
		}
*/
	_Ty *get() const _THROW0()
		{	// return wrapped pointer
		return (_Myptr);
		}

	_Ty *release() _THROW0()
		{	// return wrapped pointer and give up ownership
		_Ty *_Tmp = _Myptr;
		_Myptr = 0;
		return (_Tmp);
		}

	void reset(_Ty* _Ptr = 0)
		{	// destroy designated object and store new pointer
			// *** DIFFERENT FROM auto_ptr ***
		if (_Ptr != _Myptr)
			delete [] _Myptr;
		_Myptr = _Ptr;
		}

		// *** DIFFERENT FROM auto_ptr (new method) ***
	_Ty& operator[](int idx) const
		{
			return _Myptr[idx];
		}


private:
	_Ty *_Myptr;	// the wrapped object pointer
	};


#endif _AUTO_ARRAY_H_