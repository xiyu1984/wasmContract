#include <platon/platon.hpp>


CONTRACT AddressParam : platon::Contract {
public:
	ACTION void init(){}

	CONST int testAddressParam(platon::Address& addr)
	{
		return 73;
	}
};

PLATON_DISPATCH(AddressParam, (init)(testAddressParam))
