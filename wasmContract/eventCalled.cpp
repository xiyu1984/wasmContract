#include <platon/platon.hpp>
#include <string>

#include "dataStruct.h"

class eventCalled: public platon::Contract
{
public:
	PLATON_EVENT1(outCaller, std::string, int)

	ACTION void init()
	{

	}

	ACTION int outCall(MyStruct& ms)
	{
		PLATON_EMIT_EVENT1(outCaller, "outside", ms.i);
		return 99;
	}

};

PLATON_DISPATCH(eventCalled, (init)(outCall))
