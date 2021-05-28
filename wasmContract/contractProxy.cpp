#include <platon/platon.hpp>
#include <string>

//this proxy instance can be proxy for only one contract at a time
//real contract shall be deployed before this proxy is deployed
class calc_contract_proxy : platon::Contract{
public:

        PLATON_EVENT1(incalcAdd, std::string, int)
        PLATON_EVENT1(inmakeSum, std::string, int)

	//the input param is for Security considerations
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

	//owner can be altered. But the caller must be current contract
	ACTION bool RegisterContract(std::string& contractAddr)
	{
		//can't be called when owner is illegal
		if (!_contractAddr.self().second)
		{
			platon::internal::platon_throw("this contract init failed!");
			return false;
		}

		platon::Address senderAddr = platon::platon_caller();

		//if caller is the owner, replace the owner address
		if (senderAddr != _contractAddr.self().first)
		{
			return false;
		}

		 auto result = platon::make_address(contractAddr);
		 if (!result.second)
		 {
			 return false;
		 }

		 _contractAddr.self() = result;
		return true;
	}

	//the following methods are for represented contracts
	//the interfaces are agree with the represented contracts
	ACTION std::pair<int, bool> calcAdd(int a, int b)
	{
		//can't be called when owner is illegal
                if (!(_contractAddr.self().second))
		{
			platon::internal::platon_throw("this contract init failed!");
                        return std::pair<int, bool>(0, false);
		}

		//make call to real contract
                auto result = platon::platon_call_with_return_value<int>(_contractAddr.self().first, (unsigned int)(0), (unsigned int)(0), "calcAdd", a, b);
                PLATON_EMIT_EVENT1(incalcAdd, "calcAdd" , result.first);

		return result;
	}

        CONST std::pair<int, bool> const_calcAdd(int a, int b)
        {
            //can't be called when owner is illegal
            if (!(_contractAddr.self().second))
            {
                    platon::internal::platon_throw("this contract init failed!");
                    return std::pair<int, bool>(0, false);
            }

            //make call to real contract
            auto result = platon::platon_call_with_return_value<int>(_contractAddr.self().first, (unsigned int)(0), (unsigned int)(0), "const_calcAdd", a, b);

            return result;
        }

	ACTION std::pair<int, bool> makeSum(std::vector<int>& eles)
	{
		//can't be called when owner is illegal
                if (!(_contractAddr.self().second))
		{
			platon::internal::platon_throw("this contract init failed!");
                        return std::pair<int, bool>(0, false);
		}

		//make call to real contract
		unsigned int len = eles.size();
		if (0 == len)
		{
                        PLATON_EMIT_EVENT1(inmakeSum, "makeSum" , 0);
			return std::pair<int, bool>(0, true);
		}
		else
		{
			//call methods
                        auto result = platon::platon_call_with_return_value<int>(_contractAddr.self().first, (unsigned int)(0), (unsigned int)(0), "makeSum", eles);
                        PLATON_EMIT_EVENT1(inmakeSum, "makeSum" , result.first);
			return result;
		}
	}

private:
        platon::StorageType<"contract"_n, std::pair<platon::Address, bool>>            _contractAddr;
};

PLATON_DISPATCH(calc_contract_proxy, (init)(RegisterContract)(calcAdd)(const_calcAdd)(makeSum))
