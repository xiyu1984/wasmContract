#include <platon/platon.hpp>
#include <string>

//this proxy instance can be proxy for only one contract at a time
//real contract shall be deployed before this proxy is deployed
class calc_contract_proxy : platon::Contract{
public:
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
		if (!_contractAddr.self().second)
		{
			platon::internal::platon_throw("this contract init failed!");
			return std::pair<int, false>(0, false);
		}

		//make call to real contract
		char param[50];
		memset(param, 0, 50);
		sprintf(param, "[\"&d\", \"&d\"]", a, b);

		auto result = platon_call_with_return_value<int>(_contractAddr.self().first,
			int(0), int(0), "calcAdd", std::string(param));

		return result;
	}

	ACTION std::pair<int, bool> makeSum(std::vector<int>& eles)
	{
		//can't be called when owner is illegal
		if (!_contractAddr.self().second)
		{
			platon::internal::platon_throw("this contract init failed!");
			return std::pair<int, false>(0, false);
		}

		//make call to real contract
		unsigned int len = eles.size();
		if (0 == len)
		{
			return std::pair<int, bool>(0, true);
		}
		else
		{
			//encode params
			std::string paramStr("[");

			char eleNumber[20];
			memset(eleNumber, 0, 20);

			auto itr_ele = eles.begin();
			sprintf(eleNumber, "%d", *itr_ele);
			paramStr += std::string(eleNumber);
			++itr_ele;

			while (itr_ele != eles.end())
			{
				memset(eleNumber, 0, 20);
				sprintf(eleNumber, ", %d", *itr_ele);
				paramStr += std::string(eleNumber);
				++itr_ele;
			}

			paramStr += "]";

			//call methods
			auto result = platon_call_with_return_value<int>(_contractAddr.self().first,
				int(0), int(0), "makeSum", paramStr);

			return result;
		}
	}

private:
	platon::StorageType<"string"_n, std::pair<Address, bool>>            _contractAddr;
};

PLATON_DISPATCH(calc_contract_proxy, (init)(RegisterContract)(calcAdd)(makeSum))
