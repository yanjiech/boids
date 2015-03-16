#include "CycledList.h"
#include <cassert>

void CycledListTest::run()
{
	CycledList<int> list;
	for (int i = 0; i < 10; ++i) list.push_back(i);

	list.foreachelement2([&](CycledListElement<int>* a, CycledListElement<int>* b)
	{
		if (b->d % 2 == 0)
		{
			CycledListElement<int>* bnext = b->next;
			list.erase(a, bnext);
			list.insert(a->d * 10, a);
		}
	});

	int sum = 0;
	list.foreach([&](int x)
	{
		sum += x;
	});

	assert(list.size() == 10 && sum == 275);
	
	list.reverse();

	assert(list.ap->d == 90);

	int deleted_sum = 0;
	list.clear([&](int x)
	{
		deleted_sum += x;
	});
	assert(deleted_sum == 275);

	for (int i = 10; i < 20; ++i) list.push_back(i);

	list.foreachelement2([&](CycledListElement<int>* a, CycledListElement<int>* b)
	{
		CycledListElement<int>* bnext = b->next;
		list.erase(a, bnext);
	});

	sum = 0;
	list.foreach([&](int x)
	{
		sum += x;
	});

	assert(list.size() == 5 && sum == 70);
}