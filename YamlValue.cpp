#include "pch.h"
#include "YamlValue.h"


YamlValue::YamlValue(const YamlValue & object) : m_type(object.m_type)
{
	SetDefaultValues();
	switch (m_type)
	{
	case Int:
		m_int = object.m_int;
		break;
	case Double:
		m_double = object.m_double;
		break;
	case String:
		m_string = object.m_string;
		break;
	case Bool:
		m_bool = object.m_bool;
		break;
	case Array_Int:
		m_vectorOfInt = object.m_vectorOfInt;
		break;
	case Array_Double:
		m_vectorOfDouble = object.m_vectorOfDouble;
		break;
	case Array_String:
		m_vectorOfString = object.m_vectorOfString;
		break;
	case Array_Bool:
		m_vectorOfBool = object.m_vectorOfBool;
		break;
	case Object:
		m_object = object.m_object;
		break;
	}
}

YamlValue & YamlValue::operator=(const YamlValue & object)
{
	if (this == &object)
		return *this;

	SetDefaultValues();
	m_type = object.m_type;
	switch (m_type)
	{
	case Int:
		m_int = object.m_int;
		break;
	case Double:
		m_double = object.m_double;
		break;
	case String:
		m_string = object.m_string;
		break;
	case Bool:
		m_bool = object.m_bool;
		break;
	case Array_Int:
		m_vectorOfInt = object.m_vectorOfInt;
		break;
	case Array_Double:
		m_vectorOfDouble = object.m_vectorOfDouble;
		break;
	case Array_String:
		m_vectorOfString = object.m_vectorOfString;
		break;
	case Array_Bool:
		m_vectorOfBool = object.m_vectorOfBool;
		break;
	case Object:
		m_object = object.m_object;
		break;
	}
	return *this;
}

std::string YamlValue::GetStringRepresentationOfValue() const
{
	switch (m_type)
	{
	case Int:
		return std::to_string(m_int);
	case Double:
		return std::to_string(m_double);
	case String:
		return m_string;
	case Bool:
		return m_bool == true ? "true" : "false";
	case Array_Int:
		return GetStringFromVectorOfInt();
	case Array_Double:
		return GetStringFromVectorOfDouble();
	case Array_String:
		return GetStringFromVectorOfString();
	case Array_Bool:
		return GetStringFromVectorOfBool();
	case Null:
		return "Null";
	case Object:
		return YamlObject(m_object).Dump();
	default:
		return "";
	}
}

std::string YamlValue::GetStringFromVectorOfInt() const
{
	std::string result = "[ ";
	std::for_each(m_vectorOfInt.begin(), m_vectorOfInt.end() - 1, [&](int val) { result += std::to_string(val) + ", "; });
	result += std::to_string(*(m_vectorOfInt.end() - 1)) + " ]";

	return result;
}

std::string YamlValue::GetStringFromVectorOfDouble() const
{
	std::string result = "[ ";
	std::for_each(m_vectorOfDouble.begin(), m_vectorOfDouble.end() - 1, [&](double val) { result += std::to_string(val) + ", "; });
	result += std::to_string(*(m_vectorOfDouble.end() - 1)) + " ]";

	return result;
}

std::string YamlValue::GetStringFromVectorOfString() const
{
	std::string result = "\n";
	std::for_each(m_vectorOfString.begin(), m_vectorOfString.end() - 1, [&](std::string str) { result += "- " + str + ",\n"; });
	result += "- " + *(m_vectorOfString.end() - 1) + "\n";

	return result;
}

std::string YamlValue::GetStringFromVectorOfBool() const
{
	std::string result = "";
	std::for_each(m_vectorOfBool.begin(), m_vectorOfBool.end() - 1, [&](bool val) { result += val == true ? "\n- true, " : "\n- false, "; });
	result += *(m_vectorOfBool.end() - 1) == true ? "\n- true" : "\n- false";

	return result;
}

void YamlValue::SetDefaultValues()
{
	m_object = {};
	m_int = 0;
	m_double = 0;
	m_string = "";
	m_bool = false;
	m_vectorOfInt = {};
	m_vectorOfDouble = {};
	m_vectorOfString = {};
	m_vectorOfBool = {};
}

