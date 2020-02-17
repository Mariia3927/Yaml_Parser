#include "pch.h"
#include <iostream>

int main()
{
	const std::string data = "---\n  doe: \"a deer a female deer\"\n  ray : \"a drop of golden sun\"\n  pi : 3.14159\n  xmas : true\n  french-hens : 3\n  calling-birds :\n    -huey\n    - dewey\n    - louie\n    - fred\n---";

	YamlObject yaml(data);

	std::cout << "---" << yaml.Dump() <<"---" << '\n';
}

