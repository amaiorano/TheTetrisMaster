// SmartAssert.h
//
// from: Andrei Alexandrescu and John Torjo
// site: http://www.cuj.com/documents/s=8464/cujcexp0308alexandr/

/******************************************************************************

Example Usage:

// Optionally set the log to a file
// It can also be set to to an ostream or a function
SmartAssert::set_log("asserts.txt");

// Optionally set the handlers for each assert level to your own function
// (still logged if the log is set, though). Here we use a function
// that does nothing, so assertions are only logged.
SmartAssert::set_handler( lvl_warn, &ignore_assert);
SmartAssert::set_handler( lvl_debug, &ignore_assert);
SmartAssert::set_handler( lvl_error, &ignore_assert);
SmartAssert::set_handler( lvl_fatal, &ignore_assert);

// Basic assert usage
SMART_ASSERT(pVal != NULL); // Typical

SMART_ASSERT(a > b)(a)(b); // Will log/output value of a and b

// Logging user-friendly message (lvl_debug level as default)
SMART_ASSERT(a > b)(a)(b).msg("a must be greater than b by now");

SMART_ASSERT(a > b)(a)(b).warn("a should be greater than b, but keep going...");
SMART_ASSERT(a > b)(a)(b).debug("a must be greater than b by now");
SMART_ASSERT(a > b)(a)(b).error("a must be greater than b, throws exception");
SMART_ASSERT(a > b)(a)(b).abort("a must absolutely be greater than b, abort application");

// How about assertions that exist even in Release build? Use SMART_VERIFY, which uses
// lvl_error as default level (throws exception)
SMART_VERIFY(strUserName == "administrator")(strUserName.c_str()).msg("Security breach!");

******************************************************************************/


#ifndef _SMART_ASSERT_H_
#define _SMART_ASSERT_H_

#if _MSC_VER > 1000
// note: moving this after pragma push will render it useless (VC6)
// identifier truncated to 255 chars in debug information
#pragma warning (disable : 4786)
#pragma warning (push )
// this used in base-member initialization; it's ok
#pragma warning (disable : 4355)
#endif

// note: NEVER define SMART_ASSERT_DEBUG directly
// (it will be overridden);
//
// #define SMART_ASSERT_DEBUG_MODE instead
#ifdef SMART_ASSERT_DEBUG_MODE 
#if SMART_ASSERT_DEBUG_MODE == 1
#define SMART_ASSERT_DEBUG 
#else
#undef SMART_ASSERT_DEBUG
#endif
#else
// defaults
#ifndef NDEBUG
#define SMART_ASSERT_DEBUG 
#else
#undef SMART_ASSERT_DEBUG
#endif
#endif


#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <map>

enum
{
	// default behavior - just logs this assert
	// (a message is shown to the user to the console)
	lvl_warn = 100,

	// DEFAULT LEVEL:
	// default behavior - asks the user what to do:
	// Ignore/ Retry/ etc.
	lvl_debug = 200,

	// default behavior - throws a smart_assert_error
	lvl_error = 300,

	// default behavior - dumps all assert context to console, 
	// and aborts
	lvl_fatal = 1000
};




// contains details about a failed assertion
class assert_context 
{
	typedef std::string string;

public:
	assert_context() : level_(lvl_debug) // Debug level is default
	{
	}

	// get/set level of assertion
	void set_level(int nLevel) { level_ = nLevel; }
	int get_level() const { return level_; }

	// get/set (user-friendly) message 
	void set_level_msg(const char * strMsg)
	{ 
		if (strMsg)
			msg_ = strMsg; 
		else
			msg_.erase();
	}
	const string & get_level_msg() const { return msg_; }

	// where the assertion failed: file & line
	void set_file_line(const char * file, int line) 
	{
		file_ = file;
		line_ = line;
	}
	const string & get_context_file() const	{ return file_; }    
	int get_context_line() const			{ return line_; }

	// get/ set expression (what is being asserted)
	void set_expr(const string & str) { expr_ = str; }
	const string & get_expr() const { return expr_; }

	// variable name and value (as string) pair
	typedef std::pair< string, string> val_and_str;
	typedef std::vector< val_and_str> vals_array;

	// return values array as a vector of pairs:
	// [Value, corresponding string]
	const vals_array & get_vals_array() const { return vals_; }

	// adds one value and its corresponding string
	void add_val(const string & val, const string & str)
	{
		vals_.push_back( val_and_str(val, str) );
	}

private:
	// where the assertion occured
	string file_;
	int line_;

	// expression and values
	string expr_;
	vals_array vals_;

	// level and message
	int level_;
	string msg_;
};


namespace smart_assert 
{
	// function pointer for assertion functions
	typedef void (*assert_func)(const assert_context & context);

	// these functions are all defined in the .cpp file...

	// helpers
	std::string get_typeof_level(int nLevel);
	void dump_context_summary(const assert_context & context, std::ostream & out);
	void dump_context_detail(const assert_context & context, std::ostream & out);

	// defaults
	void default_warn_handler(const assert_context & context);
	void default_debug_handler(const assert_context & context);
	void default_error_handler(const assert_context & context);
	void default_fatal_handler(const assert_context & context);
	void default_logger(const assert_context & context);

} // namespace smart_assert

// private implementation details - must be in header for inlining
namespace smart_assert_internal 
{
	void init_assert();
	void set_default_log_stream(std::ostream & out);
	void set_default_log_name(const char * str);

	// allows finding if a value is of type 'const char *'
	// and is null; if so, we cannot print it to an ostream
	// directly!!!
	template< class T>
	struct is_null_finder
	{
		bool is (const T &) const
		{
			return false;
		}
	};

	template<>
	struct is_null_finder< char*>
	{
		bool is( char * const & val)
		{
			return val == 0;
		}
	};

	template<>
	struct is_null_finder< const char*>
	{
		bool is (const char * const & val)
		{
			return val == 0;
		}
	};
} // namespace smart_assert_internal


// The main assertion class.
// Client can use static functions in this class to setup certain parameters.
class SmartAssert
{
	typedef smart_assert::assert_func assert_func;

public: // client static functions

	// in this case, we set the default logger, and make it
	// write everything to this file
	static void set_log(const char * strFileName)
	{
		smart_assert_internal::set_default_log_name(strFileName);
		logger() = &smart_assert::default_logger;
	}

	// in this case, we set the default logger, and make it
	// write everything to this log
	static void set_log(std::ostream & out)
	{
		smart_assert_internal::set_default_log_stream(out);
		logger() = &smart_assert::default_logger;
	}

	// sets logger to input function
	static void set_log(assert_func log)
	{
		logger() = log;
	}

	// sets handler for input level
	static void set_handler(int nLevel, assert_func handler)
	{
		handlers()[nLevel] = handler;
	}

public:
	// helpers, in order to be able to compile the code (cool macro trick)
	SmartAssert& SMART_ASSERT_A;
	SmartAssert& SMART_ASSERT_B;

	SmartAssert(const char * expr) 
		: SMART_ASSERT_A(*this), 
		SMART_ASSERT_B(*this),
		needs_handling_(true) 
	{
		context_.set_expr(expr);

		if ( (logger() == 0) || handlers().size() < 4 ) 
		{
			// used before main!
			smart_assert_internal::init_assert();
		}
	}

	SmartAssert(const SmartAssert& other)
		: SMART_ASSERT_A(*this), 
		SMART_ASSERT_B(*this),
		context_(other.context_),
		needs_handling_(true)
	{
		other.needs_handling_ = false;
	}

	~SmartAssert() 
	{
		if (needs_handling_) 
			handle_assert();
	}

	template<class type>
		SmartAssert& print_current_val(const type & val, const char * msg) 
	{
		std::ostringstream out;

		smart_assert_internal::is_null_finder<type> f;
		bool bIsNull = f.is(val);
		if (!bIsNull)
			out << val;
		else            
			out << "null"; // null string
		context_.add_val(out.str(), msg);
		return *this;
	}

	SmartAssert& print_context(const char * file, int line)
	{
		context_.set_file_line(file, line);
		return *this;
	}

	SmartAssert& msg(const char * strMsg)
	{
		context_.set_level_msg(strMsg);
		return *this;
	}

	SmartAssert& level(int nLevel, const char * strMsg = 0)
	{
		context_.set_level(nLevel);
		context_.set_level_msg(strMsg);
		return *this;
	}

	SmartAssert& warn(const char * strMsg = 0)
	{
		return level(lvl_warn, strMsg);
	}

	SmartAssert& debug(const char * strMsg = 0)
	{
		return level(lvl_debug, strMsg);
	}

	SmartAssert& error(const char * strMsg = 0)
	{
		return level(lvl_error, strMsg);
	}

	SmartAssert& fatal(const char * strMsg = 0)
	{
		return level(lvl_fatal, strMsg);
	}

private:
	// handles the current assertion.
	void handle_assert()
	{
		logger()(context_);
		get_handler( context_.get_level() )(context_);
	}

	/*
	IMPORTANT NOTE:
	The only reason logger & handlers are functions, are
	because you might use SMART_ASSERT before main().

	In this case, since they're statics, they might not
	be initialized. However, making them functions 
	will make it work.
	*/

	// the log
	static assert_func & logger()
	{
		static assert_func inst;
		return inst;
	}

	// the handler
	typedef std::map< int, assert_func> handlers_collection; 
	static handlers_collection & handlers()
	{
		static handlers_collection inst;
		return inst;
	}

	static assert_func get_handler(int nLevel)
	{
		handlers_collection::const_iterator found = handlers().find(nLevel);
		if ( found != handlers().end() )
			return found->second;
		else
			// we always assume the debug handler has been set
			return handlers().find(lvl_debug)->second;
	}

private:
	assert_context context_;
	mutable bool needs_handling_;
};


// extends namespace declared above now that SmartAssert class has been declared
namespace smart_assert
{
	inline SmartAssert make_assert(const char * expr)
	{
		return SmartAssert(expr);
	}
} // namespace smart_assert



////////////////////////////////////////////////////////
// macro trickery

#ifdef SMART_ASSERT_DEBUG
// "debug" mode
#define SMART_ASSERT(expr) \
	if ( (expr) ) ; \
	else ::smart_assert::make_assert(#expr).print_context(__FILE__, __LINE__).SMART_ASSERT_A \
	/**/

#else
// "release" mode
#define SMART_ASSERT(expr) \
	if ( true ) ; \
	else ::smart_assert::make_assert("").SMART_ASSERT_A \
	/**/

#endif // ifdef SMART_ASSERT_DEBUG


#define SMART_VERIFY(expr) \
	if ( (expr) ) ; \
	else ::smart_assert::make_assert(#expr).error().print_context(__FILE__, __LINE__).SMART_ASSERT_A \
	/**/


#define SMART_ASSERT_A(x) SMART_ASSERT_OP(x, B)
#define SMART_ASSERT_B(x) SMART_ASSERT_OP(x, A)

#define SMART_ASSERT_OP(x, next) \
	SMART_ASSERT_A.print_current_val((x), #x).SMART_ASSERT_ ## next \
	/**/


#if _MSC_VER > 1000
#pragma warning ( pop )
#endif

#endif // _SMART_ASSERT_H_
