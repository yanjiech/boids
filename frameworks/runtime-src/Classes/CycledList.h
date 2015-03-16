#ifndef __Boids__CycledList__
#define __Boids__CycledList__

#include <vector>
#include <functional>

template <typename D>
class CycledListElement //优化的时候最好都使用内存池来new
{
public:
	D d;
	CycledListElement<D>* next;
	CycledListElement(D _d, CycledListElement<D>* _next = nullptr) : d(_d), next(_next)
	{
	}
};

template <typename D>
class CycledList
{
public:
	CycledListElement<D>* ap; //接入点(access point），即循环列表中的任意一个元素。方便开始访问循环链表。ap只要不被删除，是不会被改变的。
	CycledListElement<D>* tail; //新元素都会插在他后面。其实也是ap的前一个

	CycledList() : ap(nullptr), tail(nullptr), _size(0) //其实只初始化tail就够了，但为了调试的时候明显，把ap也初始化了
	{
	}

	virtual ~CycledList()
	{
		clear();
	}

	bool empty()
	{
		return tail == nullptr; //tail为空说明没有任何结点
	}

	void push_back(D d)
	{
		if (empty())
		{
			ap = new CycledListElement<D>(d);
			ap->next = ap;
			tail = ap;
		}
		else
		{
			tail->next = new CycledListElement<D>(d, ap);
			tail = tail->next;
		}
		_size++;
	}

	//在insert_after_this后面插入。返回被插入元素的所在结点
	CycledListElement<D>* insert(const D d, CycledListElement<D>* insert_after_this)
	{
		insert_after_this->next = new CycledListElement<D>(d, insert_after_this->next);
		_size++;
		if (insert_after_this == tail)
		{
			tail = insert_after_this->next;
		}
		return insert_after_this->next;
	}

	//将[to_insert_begin, to_insert_end)插入到insert_after_this之后。返回最后一个被插入元素的所在结点。begin和end相同表示插一圈，而不是什么都不插。但begin不会被重复插，所有元素只被插一次
	CycledListElement<D>* insert(CycledListElement<D>* to_insert_begin, CycledListElement<D>* to_insert_end, CycledListElement<D>* insert_after_this)
	{
		//似乎几乎能写成O(1)的，因为链表是能直接接的，不过好像我要统计size，O(1)不成了，那就直接这么来吧。。
		for (CycledListElement<D>* to_insert = to_insert_begin;;)
		{
			insert_after_this = insert(to_insert->d, insert_after_this);
			to_insert = to_insert->next;
			if (to_insert == to_insert_end) break;
		}
		return insert_after_this;
	}

	//将(erase_prev, erase_end)擦掉。即两端都是开区间。
	//最多通过prev == end删到只剩一个，全删的话得调用clear
	void erase(CycledListElement<D>* erase_prev, CycledListElement<D>* erase_end, std::function<void(D)> do_something_before_erase = nullptr)
	{
		bool tail_erased = false, ap_erased = false;
		for (CycledListElement<D>* to_delete = erase_prev->next; to_delete != erase_end;)
		{
			if (to_delete == tail)
			{
				tail_erased = true;
			}
			else if (to_delete == ap)
			{
				ap_erased = true;
			}

			if (do_something_before_erase)
			{
				do_something_before_erase(to_delete->d);
			}

			CycledListElement<D>* next_bak = to_delete->next;
			delete to_delete;
			_size--;
			to_delete = next_bak;
		}
		erase_prev->next = erase_end;

		if (tail_erased)
		{
			tail = erase_prev;
		}
		if (ap_erased)
		{
			ap = erase_end;
		}
	}

	void clear(std::function<void(D)> do_something_before_erase = nullptr)
	{
		for (CycledListElement<D>* to_delete = ap;;)
		{
			if (do_something_before_erase)
			{
				do_something_before_erase(to_delete->d);
			}

			CycledListElement<D>* next_bak = to_delete->next;
			delete to_delete;
			to_delete = next_bak;
			if (to_delete == ap) break;
		}
		ap = tail = nullptr;
		_size = 0;
	}

	//传给do_something的第一个参数要保证不被删掉
	void foreachelement(std::function<void(CycledListElement<D>*)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e);
			e = e->next;
			if (e == ap) break;
		}
	}

	//传给do_something的第一个参数要保证不被删掉
	void foreachelement2(std::function<void(CycledListElement<D>*, CycledListElement<D>*)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e, e->next);
			e = e->next;
			if (e == ap) break;
		}
	}

	//传给do_something的第一个参数要保证不被删掉
	void foreachelement2_with_break(std::function<bool(CycledListElement<D>*, CycledListElement<D>*)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			if (do_something(e, e->next)) break;
			e = e->next;
			if (e == ap) break;
		}
	}

	void foreach(std::function<void(D)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e->d);
			e = e->next;
			if (e == ap) break;
		}
	}

	void foreach_with_counter(std::function<void(D, int)> do_something)
	{
		int i = 0;
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e->d, i++);
			e = e->next;
			if (e == ap) break;
		}
	}

	void foreach2(std::function<void(D, D)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e->d, e->next->d);
			e = e->next;
			if (e == ap) break;
		}
	}

	void foreach2_with_break(std::function<bool(D, D)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			if (do_something(e->d, e->next->d)) break;
			e = e->next;
			if (e == ap) break;
		}
	}

	void foreach3(std::function<void(D, D, D)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e->d, e->next->d, e->next->next->d);
			e = e->next;
			if (e == ap) break;
		}
	}

	void foreachref(std::function<void(D&)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e->d);
			e = e->next;
			if (e == ap) break;
		}
	}

	void reverse()
	{
		std::vector<CycledListElement<D>*> elements;
		elements.reserve(_size); 
		foreachelement([&](CycledListElement<D>* e)
		{
			elements.push_back(e);
		});

		for (unsigned int i = _size - 1; i > 0; --i)
		{
			elements[i]->next = elements[i - 1];
		}
		elements.front()->next = elements.back();
		ap = elements.back();
		tail = elements.front();
	}

	int size() const
	{
		return _size;
	}

protected:
	int _size;
};

class CycledListTest
{
public:
	static void run();
};

#endif