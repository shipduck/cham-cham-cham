// Ŭnicode please 
#pragma once

#include "cbes_globals.h"

const int kDefaultPoolSize = 32;

class IFamilyComponentIdSupport {
public:
	virtual comp_id_type getFamilyId() const = 0;
	virtual comp_id_type getComponentId() const = 0;
};

class BaseComponent : public IFamilyComponentIdSupport {
public:
	virtual void setUp() = 0;
	virtual void shutDown() = 0;
	virtual void update(int ms) = 0;
};

class BaseComponentList : public IFamilyComponentIdSupport {
public:
	BaseComponentList(int poolSize=kDefaultPoolSize);
	virtual ~BaseComponentList() {}

	virtual void update(int ms) {}

	int create();
	void destroy(int compId);

	int remainPoolSize() const { return CompPool.size(); }

protected:
	std::vector<int> CompPool;
	std::vector<bool> ActiveList;
};

template<int FamilyCode, int CompCode>
class BaseComponentT : public BaseComponent {
public:
	static_assert(FamilyCode > kFamilyNone && FamilyCode < cNumFamily, "not valid family code");
	static_assert(CompCode > kCompNone && CompCode < cNumComp, "not valid comp code");

	enum {
		kFamily = FamilyCode,
		kComp = CompCode,
	};

public:
	virtual comp_id_type getFamilyId() const { return FamilyCode; }
	virtual comp_id_type getComponentId() const { return CompCode; }
};


template<int FamilyCode, int CompCode>
class BaseComponentListT : public BaseComponentList {
public:
	static_assert(FamilyCode > kFamilyNone && FamilyCode < cNumFamily, "not valid family code");
	static_assert(CompCode > kCompNone && CompCode < cNumComp, "not valid comp code");

	enum {
		kFamily = FamilyCode,
		kComp = CompCode,
	};

public:
	BaseComponentListT(int poolSize=kDefaultPoolSize) : BaseComponentList(poolSize) {}

	virtual comp_id_type getFamilyId() const { return FamilyCode; }
	virtual comp_id_type getComponentId() const { return CompCode; }
};

template<typename T>
class SimpleComponentList : public BaseComponentList {
public:
	typedef T value_type;
	enum {
		kFamily = T::kFamily,
		kComp = T::kComp,
	};

	static_assert(std::is_base_of<BaseComponent, T>::value == 1, "support only BaseComponent");
	static_assert(kFamily > kFamilyNone && kFamily < cNumFamily, "not valid family code");
	static_assert(kComp > kCompNone && kComp < cNumComp, "not valid comp code");

public:
	virtual comp_id_type getFamilyId() const { return kFamily; }
	virtual comp_id_type getComponentId() const { return kComp; }

public:
	SimpleComponentList(int poolSize=kDefaultPoolSize);
	
	T *getComp(int compId);
	int create();
	void destroy(int compId);
	void update(int ms);

public:
	std::vector<T> CompList;
};


/*
상속으로 구성되는 컴포넌트의 경우는 Component Type를 특정값 하나로 지정할수없다
그래서 이 경우는 kCompNone을 사용해서 Family Type만 명확한거로 설정
*/
template<typename T>
class InheritanceComponentList : public BaseComponentList {
public:
	typedef T value_type;
	enum {
		kFamily = T::kFamily,
		kComp = kCompNone,
	};

	static_assert(std::is_base_of<BaseComponent, T>::value == 1, "support only BaseComponent");
	static_assert(kFamily > kFamilyNone && kFamily < cNumFamily, "not valid family code");

public:
	virtual comp_id_type getComponentId() const { return kComp; }
	virtual comp_id_type getFamilyId() const { return kFamily; }

public:
	InheritanceComponentList(int poolSize=kDefaultPoolSize);
	~InheritanceComponentList();
	
	T *getComp(int compId);
	int add(T *obj);
	void destroy(int compId);
	void update(int ms);

public:
	std::vector<T*> CompList;
};

/////////////////////////////////////////
// template implement

template<typename T>
SimpleComponentList<T>::SimpleComponentList(int poolSize)
	: BaseComponentList(poolSize)
{
	CompList.resize(poolSize);
}

template<typename T>
T *SimpleComponentList<T>::getComp(int compId)
{
	SR_ASSERT(compId >= 0 && compId < (int)CompList.size());
	return &CompList[compId];
}

template<typename T>
int SimpleComponentList<T>::create()
{
	int compId = BaseComponentList::create();
	T &obj = CompList[compId];
	obj.setUp();
	return compId;
}

template<typename T>
void SimpleComponentList<T>::destroy(int compId)
{
	T &obj = CompList[compId];
	obj.shutDown();
	BaseComponentList::destroy(compId);
}
	
template<typename T>
void SimpleComponentList<T>::update(int ms)
{
	for(size_t i = 0 ; i < ActiveList.size() ; ++i) {
		int active = ActiveList[i];
		if(active == false) {
			continue;
		}
		T *comp = getComp(i);
		comp->update(ms);
	}
}

template<typename T>	
InheritanceComponentList<T>::InheritanceComponentList(int poolSize)
	: BaseComponentList(poolSize)
{
	CompList.resize(poolSize);
}

template<typename T>
InheritanceComponentList<T>::~InheritanceComponentList() 
{
	for(size_t i = 0 ; i < ActiveList.size() ; ++i) {
		int active = ActiveList[i];
		if(active == false) {
			continue;
		}
		T *comp = getComp(i);
		delete(comp);
	}
}

template<typename T>
T *InheritanceComponentList<T>::getComp(int compId)
{
	SR_ASSERT(compId >= 0 && compId < (int)CompList.size());
	return CompList[compId];
}

template<typename T>
int InheritanceComponentList<T>::add(T *obj)
{
	int compId = BaseComponentList::create();
	CompList[compId] = obj;
	obj->setUp();
	return compId;
}

template<typename T>
void InheritanceComponentList<T>::destroy(int compId)
{
	T *obj = CompList[compId];
	obj->shutDown();
	delete(obj);
	BaseComponentList::destroy(compId);
}

template<typename T>
void InheritanceComponentList<T>::update(int ms)
{
	for(size_t i = 0 ; i < ActiveList.size() ; ++i) {
		int active = ActiveList[i];
		if(active == false) {
			continue;
		}
		T *comp = getComp(i);
		comp->update(ms);
	}
}
