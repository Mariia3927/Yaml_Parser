#include "pch.h"
#include "YamlObject.h"

YamlObject::YamlObject(const std::string& str)
{
	auto iter = str.begin();
	ParseObject(str, iter);
}

std::string YamlObject::ParseKey(const std::string& str, std::string::const_iterator& curIter)
{
	auto iter = std::find_if(curIter, str.end(), [](char symbol) { return symbol == ':'; });

	while (*(iter - 1) == ' ')
		--iter;

	std::string key = str.substr(curIter - str.begin(), iter - curIter);
	curIter = iter;

	return key;
}

void YamlObject::ParseObject(const std::string & str, std::string::const_iterator& curIter)
{
	std::string key;
	YamlValue value;
	std::map<std::string, YamlValue> yaml{};

	while (*curIter == '-') { ++curIter; } //yaml file starts with three dashes
	
	if (*curIter == '\n')
		++curIter;
	m_levels.push_back(GetIndentLevel(curIter));

	while (curIter < str.end() && *curIter != '-' && *(curIter + 1) != '-' && *(curIter + 2) != '-')
	{
		if (m_levels.size() > 1 && GetIndentLevel(curIter) < m_levels[m_levels.size() - 1])
		{
			m_object = yaml;
			m_levels.pop_back();
			return;
		}
		SkipSpacesForward(curIter);
		key = ParseKey(str, curIter);

		curIter = std::find(curIter, str.end(), ':');
		++curIter;
		SkipSpacesForward(curIter);

		value = ParseValue(str, curIter);

		yaml[key] = value;

		if (*curIter == '\n')
			++curIter;
	}
	m_object = yaml;
	if (m_levels.size() > 0)
		m_levels.pop_back();
}

YamlValue YamlObject::ParseValue(const std::string & str, std::string::const_iterator& curIter)
{
	std::string value = "";
	if (isdigit(*curIter))
	{
		bool isDoubleValue = false;
		value = ParseNumericValue(str, curIter, isDoubleValue);
		return isDoubleValue == true ? YamlValue(atof(value.c_str())) : YamlValue(atoi(value.c_str()));
	}
	else if (IsBooleanValue(str, curIter))
	{
		return YamlValue(ParseBool(str, curIter));
	}
	else if (IsStringValue(str, curIter))
	{
		if (*curIter == '>' || *curIter == '|')
		{
			bool isMultiLineMode = *curIter == '>' ? false : true;
			++curIter;
			return YamlValue(ParseMultiLinedString(str, curIter, isMultiLineMode));
		}
		else
		{
			return YamlValue(ParseString(str, curIter));
		}
		
	}
	else if (*curIter == '{' || ( *curIter == '\n' && IsYamlObject(str, curIter)))
	{
		YamlValue yamlObject;
		ParseObject(str, curIter);
		yamlObject.SetObject(m_object);
		return yamlObject;
	}
	else if (*curIter == '[' || (*curIter == '\n' && IsArray(str, curIter)))
	{
		++curIter;
		return ParseArray(str, curIter);
	}
}

std::string YamlObject::ParseNumericValue(const std::string & str, std::string::const_iterator& curIter, bool& isDoubleValue) const
{
	std::string value = "";
	std::string::const_iterator tmpIter = curIter;

	while (isdigit(*curIter))
	{
		value += *curIter++;
	}

	if (*curIter == '.')
	{
		value += *curIter;
		isDoubleValue = true;
		++curIter;
	}

	while (isdigit(*curIter))
	{
		value += *curIter++;
	}

	return value;
}

bool YamlObject::ParseBool(const std::string & str, std::string::const_iterator& curIter) const
{
	auto iter = std::find_if(curIter, str.end(), [](char symbol) { return !isalnum(symbol); });

	bool value = str.substr(curIter - str.begin(), iter - curIter) == "true" ? true : false;
	curIter = iter;

	return value;
}

YamlValue YamlObject::ParseArray(const std::string & str, std::string::const_iterator& curIter) const
{
	if (*curIter == '\n' || *curIter == '[')
	{
		++curIter;
	}

	SkipSpacesForward(curIter);

	if (*curIter == '-')
		++curIter;

	SkipSpacesForward(curIter);

	if (isdigit(*curIter))
	{
		return ParseNumericArray(str, curIter);
	}
	else if (isalnum(*curIter) || *curIter == '\"' || *curIter == '\'')
	{
		if (*curIter != '\"' && *curIter != '\'' && IsBooleanValue(str, curIter))
		{
			return ParseBoolArray(str, curIter);
		}
		else
		{
			return ParseStringArray(str, curIter);
		}
	}
}

YamlValue YamlObject::ParseNumericArray(const std::string & str, std::string::const_iterator& curIter) const
{
	std::string value = "";
	bool isDoubleArray = false;
	std::vector<int> vecOfInt{};
	std::vector<double> vecOfDouble{};
	bool isNextItemExist = true;

	while (*curIter != ']' && isNextItemExist)
	{
		value = ParseNumericValue(str, curIter, isDoubleArray);
		isDoubleArray == true ? vecOfDouble.push_back(atof(value.c_str())) : vecOfInt.push_back(atoi(value.c_str()));

		SkipSpacesForward(curIter);

		if (*curIter == '\n')
			++curIter;

		SkipSpacesForward(curIter);

		if (*curIter == ',' || *curIter == '-')
		{
			isNextItemExist = true;
			++curIter;
		}
		else
		{
			isNextItemExist = false;
		}
		SkipSpacesForward(curIter);
	}
	if(*curIter == ']')
		++curIter;

	return isDoubleArray == true ? YamlValue(vecOfDouble) : YamlValue(vecOfInt);
}

void YamlObject::Replace(std::string& targetStr, char charToRemove) const
{
	targetStr.erase(std::remove(targetStr.begin(), targetStr.end(), charToRemove), targetStr.end());
}

void YamlObject::ReplaceSpaces(std::string & targetStr) const
{
	size_t idx = targetStr.find("  ", 2);
	while (idx != std::string::npos)
	{
		targetStr.replace(idx, 2, " ");
		idx = targetStr.find("  ", 2);
	}
}

YamlValue YamlObject::ParseStringArray(const std::string & str, std::string::const_iterator& curIter) const
{
	std::vector<std::string> vecOfStr {};
	std::string value;
	bool isNextItemExist = true;

	while (curIter != str.end() && isNextItemExist)
	{
		value = ParseString(str, curIter);
		
		vecOfStr.push_back(value);

		SkipSpacesForward(curIter);

		if (*curIter == '\n')
			++curIter;

		SkipSpacesForward(curIter);

		if (*curIter == ',' || (*curIter == '-' && *(curIter + 1) != '-' && *(curIter + 2) != '-'))
		{
			isNextItemExist = true;
			++curIter;
		}
		else
		{
			isNextItemExist = false;
		}
			
		SkipSpacesForward(curIter);
	}

	if (*curIter == ']')
		++curIter;

	return YamlValue(vecOfStr);
}

YamlValue YamlObject::ParseBoolArray(const std::string & str, std::string::const_iterator& curIter) const
{
	bool value = false;
	std::vector<bool> vecOfBool{};
	bool isNextItemExist = true;

	while (curIter != str.end() && isNextItemExist)
	{
		vecOfBool.push_back(ParseBool(str, curIter));

		SkipSpacesForward(curIter);

		if (*curIter == '\n')
			++curIter;

		SkipSpacesForward(curIter);

		if (*curIter == ',' || *curIter == '-')
		{
			isNextItemExist = true;
			++curIter;
		}
		else
		{
			isNextItemExist = false;
		}

		SkipSpacesForward(curIter);
	}
	if (*curIter == ']')
		++curIter;

	return YamlValue(vecOfBool);
}

std::string YamlObject::ParseMultiLinedString(const std::string & str, std::string::const_iterator& curIter, bool isMultiLineMode) const
{

	if (*curIter == '\'' || *curIter == '\"')
		++curIter;

	std::string::const_iterator iter = std::find_if(curIter, str.end(), [](char symbol) { return symbol == ':'; });

	if (iter == str.end())
	{
		while (*(iter - 1) == '-')
			--iter;
	}
	else
	{
		--iter;
		while (*(iter) == ' ' && iter != str.begin())
			--iter;
		while (*iter != ' ')
			--iter;
	}

	std::string value = str.substr(curIter - str.begin(), iter - curIter);
	curIter = iter;

	Replace(value, '\"');
	Replace(value, '\'');

	if (*curIter == '\"' || *curIter == '\'')
		++curIter;

	if (!isMultiLineMode)
	{
		Replace(value, '\n');
		ReplaceSpaces(value);
	}															
	return value;
}

std::string YamlObject::ParseString(const std::string & str, std::string::const_iterator & curIter) const
{
	std::string::const_iterator iter = std::find_if(curIter, str.end(), [](char symbol) { return symbol == '\n' || symbol == ',' || symbol == ']'; });

	std::string value = str.substr(curIter - str.begin(), iter - curIter);
	curIter = iter;

	return value;
}

void YamlObject::SkipSpacesForward(std::string::const_iterator& curIter) const
{
	while (*curIter == ' ')
	{
		++curIter;
	}
}

void YamlObject::SkipSpacesBack(std::string::const_iterator& curIter) const
{
	while (*curIter == ' ' || *curIter == '\n')
	{
		--curIter;
	}
}

YamlValue& YamlObject::operator[](const std::string& key)
{
	return m_object[key];
}

YamlValue YamlObject::at(const std::string& key)
{
	return m_object[key];
}

void YamlObject::PushBack(const std::string & key, const YamlValue & value)
{
	m_object[key] = value;
}

size_t YamlObject::Size() const 
{ 
	return m_object.size(); 
}

void YamlObject::Clear() 
{ 
	m_object.clear(); 
}

std::string YamlObject::Dump() const
{
	std::string result = "";

	for (std::map<std::string, YamlValue >::const_iterator iter = m_object.begin(); iter != m_object.end(); iter++)
	{
		result += "\n" + iter->first + " : " + iter->second.GetStringRepresentationOfValue();
	}
	result += '\n';
	return result;
}

bool YamlObject::IsDoubleValue(const std::string & str, std::string::const_iterator iter) const
{
	while (isdigit(*iter))
	{
		++iter;
	}
	return (*iter == '.' && isdigit(*(iter + 1))) ? true : false;
}

bool YamlObject::IsBooleanValue(const std::string & str, std::string::const_iterator iter) const
{
	std::string falseStr = "false", trueStr = "true";
	return (str.substr(iter - str.begin(), falseStr.length()) == falseStr || str.substr(iter - str.begin(), trueStr.length()) == trueStr) ? true : false;
}

bool YamlObject::IsStringValue(const std::string& str, std::string::const_iterator iter) const
{
	if (*iter == '\"' || *iter == '\'' || *iter == '>' || *iter == '|' || isalnum(*iter))
		return true;
	return false;
}

bool YamlObject::IsYamlObject(const std::string & str, std::string::const_iterator curIter) const
{
	if (*curIter == '\n')
		++curIter;

	if (m_levels.size() > 0 && GetIndentLevel(curIter) == 2 * m_levels[m_levels.size() - 1] && isalnum(*(curIter + 2 * m_levels[m_levels.size() - 1])))
		return true;
	else
		return false;
}

bool YamlObject::IsArray(const std::string & str, std::string::const_iterator iter) const
{
	if (*iter == '\n')
		++iter;

	if (m_levels.size() > 0 && GetIndentLevel(iter) == 2 * m_levels[m_levels.size() - 1] && *(iter + 2 * m_levels[m_levels.size() - 1]) == '-')
		return true;
	else
		return false;
}

int YamlObject::GetIndentLevel(std::string::const_iterator curIter) const
{
	int level = 0;
	while (*curIter == ' ')
	{
		++level;
		++curIter;
	}
	return level;
}