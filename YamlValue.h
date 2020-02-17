#pragma once
#include "pch.h"

class YamlObject;


class YamlValue
{
public:
	YamlValue() { SetDefaultValues(); }
	explicit YamlValue(int value) : m_type(Int), m_int(value) { }
	explicit YamlValue(double value) : m_type(Double), m_double(value) { }
	explicit YamlValue(std::string value) : m_type(String), m_string(value) { }
	explicit YamlValue(bool value) : m_type(Bool), m_bool(value) { }
	explicit YamlValue(std::vector<int> value) : m_type(Array_Int), m_vectorOfInt(value) { }
	explicit YamlValue(std::vector<double> value) : m_type(Array_Double), m_vectorOfDouble(value) { }
	explicit YamlValue(std::vector<std::string> value) : m_type(Array_String), m_vectorOfString(value) { }
	explicit YamlValue(std::vector<bool> value) : m_type(Array_Bool), m_vectorOfBool(value) { }
	explicit YamlValue(YamlValueType type) : m_type(Null) { }

	YamlValue(const YamlValue& object);
	YamlValue& operator=(const YamlValue& obj);

	std::string GetStringRepresentationOfValue() const;
	void SetObject(const std::map<std::string, YamlValue>& map) { SetDefaultValues(); m_type = Object; m_object = map; }
	
private:
	std::string GetStringFromVectorOfInt() const;
	std::string GetStringFromVectorOfDouble() const;
	std::string GetStringFromVectorOfString() const;
	std::string GetStringFromVectorOfBool() const;

	void SetDefaultValues();
	
private:
	YamlValueType m_type;

	std::map<std::string, YamlValue> m_object{};
	int m_int = 0;
	double m_double = 0.0;
	std::string m_string = "";
	bool m_bool = false;
	std::vector<int> m_vectorOfInt{};
	std::vector<double> m_vectorOfDouble{};
	std::vector<std::string> m_vectorOfString{};
	std::vector<bool> m_vectorOfBool{};
};
