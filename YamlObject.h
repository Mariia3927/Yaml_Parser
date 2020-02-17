#pragma once
#include "pch.h"

class YamlValue;

class YamlObject
{
public:
	YamlObject() {}
	explicit YamlObject(const std::string& str);
	explicit YamlObject(const std::map<std::string, YamlValue>& map) : m_object(map) {}

	YamlValue& operator[](const std::string& key);
	YamlValue at(const std::string& key);

	void PushBack(const std::string& key, const YamlValue& value);
	size_t Size() const { return m_object.size(); }
	void Clear() { m_object.clear(); }
	std::string Dump() const;

private:
	std::string ParseKey(const std::string& str, std::string::const_iterator& curIter);
	void ParseObject(const std::string& str, std::string::const_iterator& curIter);
	YamlValue ParseValue(const std::string& str, std::string::const_iterator& curIter);
	std::string ParseNumericValue(const std::string& str, std::string::const_iterator& curIter, bool& isDoubleValue) const;
	bool ParseBool(const std::string& str, std::string::const_iterator& curIter) const;
	YamlValue ParseArray(const std::string& str, std::string::const_iterator& curIter) const;
	YamlValue ParseNumericArray(const std::string& str, std::string::const_iterator& curIter) const;
	YamlValue ParseStringArray(const std::string& str, std::string::const_iterator& curIter) const;
	YamlValue ParseBoolArray(const std::string& str, std::string::const_iterator& curIter) const;
	std::string ParseMultiLinedString(const std::string& str, std::string::const_iterator& curIter, bool isMultiLineMode) const;
	std::string ParseString(const std::string& str, std::string::const_iterator& curIter) const;

	void SkipSpacesForward(std::string::const_iterator& curIter) const;
	void SkipSpacesBack(std::string::const_iterator& curIter) const;
	void Replace(std::string& targetStr, char charToRemove) const;
	void ReplaceSpaces(std::string& targetStr) const;

	bool IsDoubleValue(const std::string& str, std::string::const_iterator iter) const;
	bool IsBooleanValue(const std::string& str, std::string::const_iterator iter) const;
	bool IsStringValue(const std::string& str, std::string::const_iterator iter) const;
	bool IsYamlObject(const std::string & str, std::string::const_iterator iter) const;
	bool IsArray(const std::string & str, std::string::const_iterator iter) const;

	int GetIndentLevel(std::string::const_iterator curIter) const;
	
private:
	std::map<std::string, YamlValue> m_object{};
	std::vector<int> m_levels {2};
};
