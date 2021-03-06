#include <platon/platon.hpp>

#include <string>
#include <vector>

CONTRACT calc_contract : public platon::Contract
{
public:
	ACTION void init()
	{
		//the owner of the contract is best to be the operator of the deployment
		//in this instance, owner address can not be changed
		_ownerAddr.self() = std::pair<platon::Address, bool>(platon::platon_caller(), true);
	}

	//this methods shall be called only after the proxy contract is deployed
	ACTION bool RegisterProxy(const std::string& proxyAddr)
	{
		//set and register the proxy address
		auto p_Addr = platon::make_address(proxyAddr);
		if (!p_Addr.second)
		{
			_proxyAddr.self() = std::pair<platon::Address, bool>(platon::Address(), false);
			platon::internal::platon_throw("register proxy failed! illegal proxy address!");
			return false;
		}
		else
		{
			_proxyAddr.self() = p_Addr;
            return true;
		}
	}

    CONST std::string GetProxyAddress()
    {
        return _proxyAddr.self().first.toString();
    }

    CONST std::string GetOwnerAddress()
    {
        return _ownerAddr.self().first.toString();
    }

	//methods for proxy mechanism
	//the param is the next contract address the proxy really use
	ACTION bool updateContract(const std::string& contractAddr)
	{
		//only owner can updateContract
		auto send_Addr = platon::platon_caller();
		if (_ownerAddr.self().first != send_Addr)
		{
			return false;
		}

		//check the contract address
		auto c_Addr = platon::make_address(contractAddr);
		if (!c_Addr.second)
		{
			return false;
		}

		//call proxy
		std::string paramStr = "[\"";
		paramStr += contractAddr;
		paramStr += "\"]";

		if (!_proxyAddr.self().second)
		{
			return false;
		}

        auto result = platon::platon_call_with_return_value<bool>(_proxyAddr.self().first, (unsigned int)(0), (unsigned int)(0), "RegisterOwner", paramStr);
		return result.second;
	}

	//calculation methods
	ACTION int calcAdd(int a, int b)
	{
		return a + b;
	}

    CONST int const_calcAdd(int a, int b)
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
	
	platon::StorageType<"proxy"_n, std::pair<platon::Address, bool>>			_proxyAddr;
};

PLATON_DISPATCH(calc_contract, (init)(RegisterProxy)(GetProxyAddress)(GetOwnerAddress)(updateContract)(calcAdd)(const_calcAdd)(makeSum))
