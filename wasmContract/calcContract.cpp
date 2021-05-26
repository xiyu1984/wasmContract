#include <platon/platon.hpp>

#include <string>
#include <vector>

class calc_contract : public platon::Contract
{
public:
	ACTION void init(const std::string& proxyAddr)
	{
		//set owner address
		_ownerAddr.self() = std::pari<platon::Address, bool>(platon::platon_caller(), true);
		_proxyAddr.self() = proxyAddr;
	}

	//methods for proxy mechanism
	//the param is the next contract address the proxy really use
	ACTION bool updateContract(const std::string& contractAddr)
	{
		auto c_Addr = platon::make_address(contractAddr);
		if (!c_Addr.second)
		{
			return false;
		}

		auto send_Addr = platon::platon_caller();

		if (_ownerAddr.self().first != send_Addr)
		{
			return false;
		}

		//call proxy

	}

	//the param is the next owner of this contract
	ACTION bool replaceOwner(const std::string& ownerAddr)
	{
		auto o_Addr = platon::make_address(ownerAddr);
		if (!o_Addr.second)
		{
			return false;
		}

		auto send_Addr = platon::platon_caller();

		if (_ownerAddr.self().first != send_Addr)
		{
			return false;
		}

		_ownerAddr.self() = o_Addr;
	}

	//calculation methods
	ACTION int calcAdd(int a, int b)
	{
		return a + b;
	}

	ACTION int makeSum(std::vector<int>& eles)
	{
		int rst = 0;
		for (auto itr = eles.begin(); itr != eles.end(); ++itr)
		{
			rst += *itr;
		}

		return rst;
	}

private:
	//contracts using proxy mechanism are needed to using owner address principle.
	platon::StorageType<"owner"_n, std::pair<platon::Address, bool>>            _ownerAddr;
	
	platon::StorageType<"string"_n, std::string>								_proxyAddr;
};

PLATON_DISPATCH(calc_contract, (init))
