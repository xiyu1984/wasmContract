#include <platon/platon.hpp>
#include <string>

class outCaller : public platon::Contract
{
public:
	ACTION void init(std::string& contractAddr)
	{
		auto rst = platon::make_address(contractAddr);
		if (!rst.second)
		{
			//when the contractAddr is illegal, the proxy can not be used
			platon::internal::platon_throw("deploy failed!");
		}
		else
		{
			_contractAddr.self() = rst;
		}
	}

	ACTION void setContract(std::string& contractAddr)
	{
		auto rst = platon::make_address(contractAddr);
		if (!rst.second)
		{
			//when the contractAddr is illegal, the proxy can not be used
			platon::internal::platon_throw("deploy failed!");
		}
		else
		{
			_contractAddr.self() = rst;
		}
	}

	CONST std::string getContract()
	{
		_contractAddr.self().first.toString();
	}

	ACTION int Call()
	{
		if (!(_contractAddr.self().second))
		{
			platon::internal::platon_throw("this contract init failed!");
			return std::pair<int, bool>(0, false);
		}

		auto result = platon::platon_call_with_return_value<int>(_contractAddr.self().first, (unsigned int)(0), (unsigned int)(0), "outCall");
		return result;
	}

	platon::StorageType<"contract"_n, std::pair<platon::Address, bool>>            _contractAddr;

};

PLATON_DISPATCH(eventCalled, (init)(Call))
