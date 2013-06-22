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
