// Ŭnicode please 
#pragma once

#include "Typelist.h"

template<class TList>
class GenSimpleHierarchy;

template<class T1, class T2>
class GenSimpleHierarchy< Loki::Typelist<T1, T2> > :
	public GenSimpleHierarchy<T1>,
	public GenSimpleHierarchy<T2> {
public:
	typedef Loki::Typelist<T1, T2> TList;
	typedef GenSimpleHierarchy<T1> LeftBase;
	typedef GenSimpleHierarchy<T2> RightBase;
};

template<class T>
class GenSimpleHierarchy : public T { };

template<>
class GenSimpleHierarchy<Loki::NullType> { };

template<typename T>
void safeDelete(T* &ptr) 
{
	if(ptr != nullptr) {
		delete(ptr);
		ptr = nullptr;
	}
}

template<typename T>
void safeDeleteArray(T* &ptr) 
{
	if(ptr != nullptr) {
		delete[](ptr);
		ptr = nullptr;     
	}
}

template<typename T>
void safeDeleteWithNullCheck(T* &ptr) 
{
	SR_ASSERT(ptr != nullptr && "null지우기는 언어상은 지원하나 아마도 잘못 짜여져있을 가능성이 있다");
	delete(ptr);
	ptr = nullptr;
}

template<typename T>
void safeDeleteArrayWithNullCheck(T* &ptr) 
{
	SR_ASSERT(ptr != nullptr && "null지우기는 언어상은 지원하나 아마도 잘못 짜여져있을 가능성이 있다");
	delete[](ptr);
	ptr = nullptr;
}

template<typename T, int N>
int getArraySize(T (&arr)[N]) 
{
	return N;
}

template<typename T>
void callConstructor(T *ptr) 
{
	new(ptr) T;
}

template<typename T>
void callDestructor(T *ptr) 
{
	ptr->~T();
}