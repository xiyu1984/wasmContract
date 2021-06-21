#pragma once

#include <platon/platon.hpp>
#include <string>

struct MyStruct
{
	int i;
	std::string s;

	PLATON_SERIALIZE(MyStruct, (i)(s))

	MyStruct()
	{
		i = 0;
		s = "";
	}
};
