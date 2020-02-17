#ifndef PCH_H
#define PCH_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>

enum YamlValueType
{
	Int, Double, String, Bool, Array_Int, Array_Double, Array_String, Array_Bool, Object, Null
};

#include "YamlObject.h"
#include "YamlValue.h"

#endif //PCH_H
