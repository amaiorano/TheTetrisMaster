#include "Config.h"

#include <fstream>
#include <strstream>
using namespace std;

void Config::AddVar(const std::string& strVarName, const Value& defVal)
{	
	// If entry not already added, then add it to the list for ordering output during save
	if ( m_configMap.find(strVarName) == m_configMap.end() )
		m_configVarNameList.push_back(strVarName);

	(*this)[strVarName] = defVal; // Add to map
}

bool Config::LoadConfigFile(const std::string& strFileName, bool bSaveFile/*=true*/)
{
	ifstream fin(strFileName.c_str());
	if ( !fin.fail() )
	{
		// Config file format is simple: one variable/value pair per line
		char buff[1024];
		while ( fin.getline(buff, sizeof(buff)) )
		{
			strstream lineStream(buff, static_cast<streamsize>(strlen(buff)));

			string strVarName;
			lineStream >> strVarName; // Extract variable name

			string strValue = lineStream.str(); // Grab the rest of the line as a string
			str_trim(strValue); // Trim surrounding whitespace

			if ( strVarName.length()>0 && strValue.length()>0 )
				(*this)[strVarName] = Value(strValue);
		}

		fin.close(); // Done reading from the file
	}	

	if ( bSaveFile )
		SaveConfigFile(strFileName);

	return true;
}

void Config::SaveConfigFile(const std::string& strFileName)
{
	ofstream fout(strFileName.c_str());

	// We iterate through the variable name list so that the values
	// are saved in the fixed order that the variables were added.
	StringList::const_iterator it;
	for (it = m_configVarNameList.begin(); it != m_configVarNameList.end(); ++it)
	{
		StringValueMap::iterator mapIter = m_configMap.find((*it));

		fout << mapIter->first << " " << mapIter->second.AsString() << endl;
	}
}

const Value& Config::operator[](const std::string& strVarName) const
{
	return m_configMap.find(strVarName)->second;
}

Value& Config::operator[](const std::string& strVarName)
{
	return m_configMap[strVarName];
}
