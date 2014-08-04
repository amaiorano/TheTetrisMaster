#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <map>
#include <vector>
#include "string_helpers.h"

// Represents a generic value of either type int, string.
class Value
{
public:
	// Default constructor needed for std::map and such
	Value() : m_strValue("No Value") {}

	// Non-explicit constructors
	Value(int iVal) { str_format_into(m_strValue, "%d", iVal); }
	Value(const std::string& strValue) : m_strValue(strValue) {}
	Value(const char* pszValue) : m_strValue(pszValue) {}

	// Explicit conversions
	const std::string& AsString() const	{ return m_strValue; }
	const int AsInt() const				{ return atoi(m_strValue.c_str()); }

	// Compares two Value objects. Note that due to non-explicit constructors, you
	// can easily do the following:
	// Value v("hello");
	// if ( v == "bye" ) // Implicitly constructs Value("bye") then calls Value::operator==()
	bool Compare(const Value& rValue) const { return m_strValue.compare(rValue.AsString())==0; }
	bool operator==(const Value& rValue) const { return Compare(rValue); }

private:
	std::string m_strValue;
};


// Use this class to load/save configuration parameters from/to a file.
// The config file format is very simple: VariableName VariableValue
// The value can be an integer or a string (strings can contain spaces).
class Config
{
public:
	typedef std::string str;

	// Before loading up the file, add expected list of variables with defaults
	void AddVar(const std::string& strVarName, const Value& defVal);

	// Loads up the config file. If the file does not exist or variables are
	// missing, the default values will be used. If bSaveFile is true, the currently
	// loaded values (including defaults) will be saved immediately to the file.
	bool LoadConfigFile(const std::string& strFileName, bool bSaveFile=true);

	// Saves current values to the config file
	void SaveConfigFile(const std::string& strFileName);

	// Accessors for the loaded values
	const Value& operator[](const std::string& strVarName) const; // Accessor (rhs)

	// Mutator (lhs)
	Value& operator[](const std::string& strVarName);

private:
	typedef std::vector<std::string> StringList;
	StringList m_configVarNameList;

	typedef std::map<std::string, Value> StringValueMap;
	StringValueMap m_configMap;
};

#endif // _CONFIG_H_
