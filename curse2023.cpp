#include <iostream>
#include <utility>
#include <new>
#include <array>

enum class typetag: char
{
	money = 'M',
	gold = 'G',
};

struct Money
{
	std::array<char,6> data = {};
	typetag tag = typetag::money;
};

struct Gold
{
	typetag tag = typetag::gold;
	float weight = 1.0f;
};

class bank 
{
	char vault[1024]{};
	char* firstFree = vault;

public:
	constexpr 
	typetag* deposit(typetag* worth)
	{
		switch (*worth)
		{
		case typetag::money:
		{
			auto money = std::launder((Money*)((char*)worth - offsetof(Money, tag)));
			auto newMoney = new(firstFree) Money(*money);
			delete money;
			firstFree += sizeof(Money);
			return &newMoney->tag;
		}
		case typetag::gold:
		{
			auto money = std::launder((Gold*)((char*)worth - offsetof(Gold, tag)));
			auto newMoney = new(firstFree) Gold(*money);
			delete money;
			firstFree += sizeof(Gold);
			return &newMoney->tag;
		}
		default:
			break;
		}
		//std::unreachable();
		return nullptr;
	}
	constexpr
	typetag* withdraw(typetag* t)
	{
		switch (*t)
		{
			case typetag::money:
			{
				auto money = std::launder((Money*)(t - offsetof(Money, tag)));
				auto newmoney = new Money(*money);
				money->~Money();
				return &newmoney->tag;
			}
			case typetag::gold:
			{
				auto money = std::launder((Gold*)(t - offsetof(Gold, tag)));
				auto newmoney = new Gold(*money);
				money->~Gold();
				return &newmoney->tag;
			}
		}
		return t;
	}
};

bank b;

typetag* funca()
{
	Money* m = new Money();
	//m->data = std::to_array("\U0001F47B\0\0\0\0");
	m->data = std::to_array("ghost");
	auto money = b.deposit(&m->tag);
}

void funcb(typetag*);
int main()
{
	funcb(funca());
}

void funcb(typetag* money)
{
	auto m2 = b.withdraw(money);
	std::cout << &std::launder((Money*)(money - offsetof(Money, tag)))->data;
}
