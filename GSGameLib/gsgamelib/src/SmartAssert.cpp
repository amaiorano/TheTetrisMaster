// SmartAssert.cpp

#include "SmartAssert.h"

#include <fstream>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <stdexcept>

#ifdef WIN32
#include <Windows.h>
#endif

void break_into_debugger()
{
	// MSVC, BCB, 
#if (defined _MSC_VER) || (defined __BORLANDC__)
	__asm { int 3 };
#elif defined(__GNUC__)
	// GCC
	__asm ("int $0x3");
#else
#  error Please supply instruction to break into code
#endif
}

namespace // anonymous namespace
{
	// in case we're logging using the default logger...
	struct stream_holder 
	{
		stream_holder() : out_(0), owns_(false) {}
		~stream_holder()
		{
			if (owns_)
				delete out_;
			out_ = 0;
		}
		std::ostream * out_;
		bool owns_;
	};

	// information about the stream we write to, in case 
	// we're using the default logger
	stream_holder default_logger_info;

	// intitializes the SMART_ASSERT library
	struct assert_initializer 
	{
		assert_initializer()
		{
			smart_assert_internal::init_assert();
		}
	} init;

} // anonymous namespace



namespace smart_assert
{

	// returns a message corresponding to the type of level
	std::string get_typeof_level(int nLevel)
	{
		switch (nLevel)
		{
		case lvl_warn: return "Warning";
		case lvl_debug: return "Assertion failed";
		case lvl_error: return "Assertion failed (Error)";
		case lvl_fatal: return "Assertion failed (FATAL)";
		default:
			{
				std::ostringstream out;
				out << "Assertion failed (level=" << nLevel << ")";
				return out.str();
			}
		};
	}

	// helpers, for dumping the assertion context
	void dump_context_summary(const assert_context & context, std::ostream & out)
	{
		out << std::endl << get_typeof_level( context.get_level() ) 
			<< " in " << context.get_context_file() << ":" << context.get_context_line() << std::endl;
		if ( !context.get_level_msg().empty() )
			// we have a user-friendly message
			out << "Message: " << context.get_level_msg();
		else
			out << "Expression: " << context.get_expr();
		out << std::endl;
	}

	void dump_context_detail(const assert_context & context, std::ostream & out)
	{
		out << std::endl << get_typeof_level( context.get_level() )
			<< " in " << context.get_context_file() << ":" << context.get_context_line() << std::endl;

		if ( !context.get_level_msg().empty() )
			out << "Message: '" << context.get_level_msg() << "'" << std::endl;

		out << "Expression: '" << context.get_expr() << "'" << std::endl;

		typedef assert_context::vals_array vals_array;
		const vals_array & aVals = context.get_vals_array();
		if ( !aVals.empty() )
		{
			out << "Values:";

			vals_array::const_iterator first = aVals.begin(), last = aVals.end();
			while (first != last)
			{
				out << " " << first->second <<  "='" << first->first << "'";
				++first;

				if ( first != last )
					out << ", ";
			}
			out << std::endl;
		}
		//        out << std::endl;
	}

	///////////////////////////////////////////////////////
	// logger

	void default_logger(const assert_context & context)
	{
		if (default_logger_info.out_ == 0)
			return;
		dump_context_detail( context, *(default_logger_info.out_) );
	}

	///////////////////////////////////////////////////////
	// handlers

	// warn : just dump summary to console
	void default_warn_handler(const assert_context & context)
	{
		dump_context_summary(context, std::cout);
	}


	// debug: ask user what to do
	void default_debug_handler(const assert_context & context)
	{
		static bool ignore_all = false;

		if (ignore_all)            
			return; // ignore All asserts

		typedef std::pair< std::string, int> file_and_line;
		static std::set< file_and_line> ignorer;
		if ( ignorer.find( file_and_line( context.get_context_file(), context.get_context_line())) != ignorer.end() )
			return; // this is Ignored Forever

		char ch = 0;

		std::stringstream ssContextDetail;
		dump_context_detail(context, ssContextDetail);
		//      dump_context_detail( context, std::cerr );
		//      dump_context_summary( context, std::cerr );

#ifdef WIN32
		// In Windows, pop a message box

		int msgResponse = ::MessageBox(NULL, ssContextDetail.str().c_str(), "ASSERT FAILED", MB_ABORTRETRYIGNORE|MB_ICONERROR|MB_APPLMODAL|MB_TOPMOST);

		switch (msgResponse)
		{
		case IDABORT:   ch = 'B'; break; // Abort
		case IDRETRY:   ch = 'D'; break; // Debug
		default:        ch = 'I'; break; // Ignore
		}
#else
		// In non-windows, we actually get more options :)

		std::cerr << ssContextDetail;
		std::cerr << std::endl << "Press (I)gnore / Ignore (F)orever / Ignore (A)ll / (D)ebug / A(b)ort: ";
		std::cerr.flush();

		std::cin.get(ch)
#endif

			switch (ch) 
		{
			case 'i': case 'I':
				// ignore
				break;

			case 'f': case 'F':
				// ignore forever
				ignorer.insert( file_and_line( context.get_context_file(), context.get_context_line()) );
				break;

			case 'a': case 'A':
				// ignore all
				ignore_all = true;
				break;

			case 'b': case 'B':
				abort();
				break;

			case 'd': case 'D':
			default:
				// break
				break_into_debugger();
				break;
		}
	}


	// error : throw a runtime exception
	void default_error_handler(const assert_context & context)
	{
		std::ostringstream out;
		dump_context_summary(context, out);
		throw std::runtime_error(out.str());
	}


	// fatal : dump error and abort
	void default_fatal_handler(const assert_context & context)
	{
		dump_context_detail(context, std::cerr);
		abort();
	}


} // namespace smart_assert


namespace smart_assert_internal
{

	void init_assert()
	{
		SmartAssert::set_log( &::smart_assert::default_logger );
		SmartAssert::set_handler( lvl_warn, &::smart_assert::default_warn_handler );
		SmartAssert::set_handler( lvl_debug, &::smart_assert::default_debug_handler );
		SmartAssert::set_handler( lvl_error, &::smart_assert::default_error_handler );
		SmartAssert::set_handler( lvl_fatal, &::smart_assert::default_fatal_handler );
	}

	// sets the default logger to write to this stream
	void set_default_log_stream(std::ostream & out)
	{
		default_logger_info.out_ = &out;
		default_logger_info.owns_ = false;
	}

	// sets the default logger to write to this file
	void set_default_log_name(const char * str)
	{
		default_logger_info.owns_ = false;
		default_logger_info.out_ = new std::ofstream(str);
		default_logger_info.owns_ = true;
	}


} // namespace smart_assert_internal

