#ifndef __Boids__CycledList__
#define __Boids__CycledList__

#include <vector>
#include <functional>

template <typename D>
class CycledListElement //”≈ªØµƒ ±∫Ú◊Ó∫√∂º π”√ƒ⁄¥Ê≥ÿ¿¥new
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
	CycledListElement<D>* ap; //Ω”»Îµ„(access point£©£¨º¥—≠ª∑¡–±Ì÷–µƒ»Œ“‚“ª∏ˆ‘™Àÿ°£∑Ω±„ø™ º∑√Œ —≠ª∑¡¥±Ì°£ap÷ª“™≤ª±ª…æ≥˝£¨ «≤ªª·±ª∏ƒ±‰µƒ°£
	CycledListElement<D>* tail; //–¬‘™Àÿ∂ºª·≤Â‘⁄À˚∫Û√Ê°£∆‰ µ“≤ «apµƒ«∞“ª∏ˆ

	CycledList() : ap(nullptr), tail(nullptr), _size(0) //∆‰ µ÷ª≥ı ºªØtailæÕπª¡À£¨µ´Œ™¡Àµ˜ ‘µƒ ±∫Ú√˜œ‘£¨∞—ap“≤≥ı ºªØ¡À
	{
	}

	virtual ~CycledList()
	{
		clear();
	}

	bool empty()
	{
		return tail == nullptr; //tailŒ™ø’Àµ√˜√ª”–»Œ∫ŒΩ·µ„
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

	//‘⁄insert_after_this∫Û√Ê≤Â»Î°£∑µªÿ±ª≤Â»Î‘™ÀÿµƒÀ˘‘⁄Ω·µ„
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

	//Ω´[to_insert_begin, to_insert_end)≤Â»ÎµΩinsert_after_this÷Æ∫Û°£∑µªÿ◊Ó∫Û“ª∏ˆ±ª≤Â»Î‘™ÀÿµƒÀ˘‘⁄Ω·µ„°£begin∫Õendœ‡Õ¨±Ì æ≤Â“ª»¶£¨∂¯≤ª « ≤√¥∂º≤ª≤Â°£µ´begin≤ªª·±ª÷ÿ∏¥≤Â£¨À˘”–‘™Àÿ÷ª±ª≤Â“ª¥Œ
	CycledListElement<D>* insert(CycledListElement<D>* to_insert_begin, CycledListElement<D>* to_insert_end, CycledListElement<D>* insert_after_this)
	{
		//À∆∫ıº∏∫ıƒ‹–¥≥…O(1)µƒ£¨“ÚŒ™¡¥±Ì «ƒ‹÷±Ω”Ω”µƒ£¨≤ªπ˝∫√œÒŒ““™Õ≥º∆size£¨O(1)≤ª≥…¡À£¨ƒ«æÕ÷±Ω”’‚√¥¿¥∞…°£°£
		for (CycledListElement<D>* to_insert = to_insert_begin;;)
		{
			insert_after_this = insert(to_insert->d, insert_after_this);
			to_insert = to_insert->next;
			if (to_insert == to_insert_end) break;
		}
		return insert_after_this;
	}

	//Ω´(erase_prev, erase_end)≤¡µÙ°£º¥¡Ω∂À∂º «ø™«¯º‰°£
	//◊Ó∂‡Õ®π˝prev == end…æµΩ÷ª £“ª∏ˆ£¨»´…æµƒª∞µ√µ˜”√clear
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

	//¥´∏¯do_somethingµƒµ⁄“ª∏ˆ≤Œ ˝“™±£÷§≤ª±ª…æµÙ
	void foreachelement(std::function<void(CycledListElement<D>*)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e);
			e = e->next;
			if (e == ap) break;
		}
	}

	//¥´∏¯do_somethingµƒµ⁄“ª∏ˆ≤Œ ˝“™±£÷§≤ª±ª…æµÙ
	void foreachelement2(std::function<void(CycledListElement<D>*, CycledListElement<D>*)> do_something)
	{
		for (CycledListElement<D>* e = ap;;)
		{
			do_something(e, e->next);
			e = e->next;
			if (e == ap) break;
		}
	}

	//¥´∏¯do_somethingµƒµ⁄“ª∏ˆ≤Œ ˝“™±£÷§≤ª±ª…æµÙ
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