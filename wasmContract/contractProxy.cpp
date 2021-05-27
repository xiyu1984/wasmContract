#include <platon/platon.hpp>
#include <string>

//this proxy instance can be proxy for only one contract at a time
class calc_contract_proxy : platon::Contract{
public:
	//the input param is for Security considerations
	ACTION void init(std::string& contractAddr)
	{
		_ownerAddr.self() = platon::make_address(contractAddr);
	}

	//owner can be altered. But the caller must be current owner
	ACTION bool RegisterOwner(std::string& contractAddr)
	{
		platon::Address senderAddr = platon::platon_caller();

		//if caller is the owner, replace the owner address
		if (senderAddr == _ownerAddr.self().first)
		{
			_ownerAddr.self() = platon::make_address(contractAddr);
		}
		
		return true;
	}

	//the following methods are for represented contracts
	//the interfaces are agree with the represented contracts
	ACTION std::pair<int, bool> calcAdd(int a, int b)
	{
		char param[50];
		memset(param, 0, 50);
		sprintf(param, "[\"&d\", \"&d\"]", a, b);

		return result = platon_call_with_return_value<int>(_ownerAddr.self().first,
			int(0), int(0), "calcAdd", std::string(param));
	}

	ACTION std::pair<int, bool> makeSum(std::vector<int>& eles)
	{
		int len = eles.size();
		if (0 == len)
		{
			return std::pair<int, bool>(0, true);
		}
		else if (0 > len)
		{
			return std::pair<int, bool>(0, false);
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
			return result = platon_call_with_return_value<int>(_ownerAddr.self().first,
				int(0), int(0), "makeSum", paramStr);
		}
	}

private:
	platon::StorageType<"string"_n, std::pair<Address, bool>>            _ownerAddr;
};

PLATON_DISPATCH(calc_contract_proxy, (init)(calcAdd)(makeSum))
