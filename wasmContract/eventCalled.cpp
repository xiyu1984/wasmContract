#include <platon/platon.hpp>
#include <string>

class eventCalled: public platon::Contract
{
public:
	PLATON_EVENT1(outCaller, std::string, int)

	ACTION void init()
	{

	}

	ACTION int outCall()
	{
		PLATON_EMIT_EVENT1(outCaller, "outside", 99);
		return 99;
	}

};

PLATON_DISPATCH(eventCalled, (init)(outCall))
