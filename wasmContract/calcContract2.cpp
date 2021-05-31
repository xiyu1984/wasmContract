#include <platon/platon.hpp>

#include <string>
#include <vector>

class calc_contract2 : public platon::Contract
{
public:
	ACTION void init()
	{
		//the owner of the contract is best to be the operator of the deployment
		//in this instance, owner address can not be changed
                _ownerAddr.self() = std::pair<platon::Address, bool>(platon::platon_caller(), true);

                //init the proxy
                _proxyAddr.self() = std::pair<platon::Address, bool>(platon::Address(), false);
	}

	//this methods shall be called only after the proxy contract is deployed
	ACTION bool RegisterProxy(const std::string& proxyAddr)
	{
                //only owner can Register Proxy
                auto send_Addr = platon::platon_caller();
                if (_ownerAddr.self().first != send_Addr)
                {
                        return false;
                }

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
            if (_proxyAddr.self().second)
            {
                return _proxyAddr.self().first.toString();
            }
            else {
                return "proxy not initiallized!";
            }
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
		if (!_proxyAddr.self().second)
		{
			return false;
		}

                auto result = platon::platon_call_with_return_value<bool>(_proxyAddr.self().first, (unsigned int)(0), (unsigned int)(0), "RegisterContract", contractAddr);

                if (!result.second)
                {
                    return false;
                }

                //clear the proxy address
                _proxyAddr.self() = std::pair<platon::Address, bool>(platon::Address(), false);

		return result.second;
	}

        //calculation methods, the interface must be the same with the proxy
	ACTION int calcAdd(int a, int b)
	{
            if (!_proxyAddr.self().second)
            {
                    return -999999;
            }

            //only proxy could call
            auto send_Addr = platon::platon_caller();
            if (_proxyAddr.self().first != send_Addr)
            {
                return -999999;
            }

            //be different with contract1
            return a + b + 1000000;
	}

        CONST int const_calcAdd(int a, int b)
        {
            if (!_proxyAddr.self().second)
            {
                    return -999999;
            }

            //only proxy could call
            auto send_Addr = platon::platon_caller();
            if (_proxyAddr.self().first != send_Addr)
            {
                return -999999;
            }

            return a + b + 1000000;
        }

	ACTION int makeSum(std::vector<int>& eles)
	{
            if (!_proxyAddr.self().second)
            {
                    return -999999;
            }

            //only proxy could call
            auto send_Addr = platon::platon_caller();
            if (_proxyAddr.self().first != send_Addr)
            {
                return -999999;
            }

            int rst = 0;
            for (auto itr = eles.begin(); itr != eles.end(); ++itr)
            {
                rst += *itr;
            }

            //be different with contract1
            return rst + 1000000;
	}

private:
	//contracts using proxy mechanism are needed to using owner address principle.
	platon::StorageType<"owner"_n, std::pair<platon::Address, bool>>            _ownerAddr;
	
	platon::StorageType<"proxy"_n, std::pair<platon::Address, bool>>			_proxyAddr;
};

PLATON_DISPATCH(calc_contract2, (init)(RegisterProxy)(GetProxyAddress)(GetOwnerAddress)(updateContract)(calcAdd)(const_calcAdd)(makeSum))
