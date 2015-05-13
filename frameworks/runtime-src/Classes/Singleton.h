//
//  Singleton.h
//
//

#ifndef __Singleton_h__
#define __Singleton_h__

template <typename T>
class Singleton
{
public:
	static T* getInstance()
	{
		if (!_pInstance) _pInstance = new T;
		return _pInstance;
	}
protected:
	Singleton() {}
private:
	Singleton(Singleton const&);
	Singleton operator=(Singleton const&);
	static T* _pInstance;
};

template <class T> T* Singleton<T>::_pInstance = nullptr;

#endif