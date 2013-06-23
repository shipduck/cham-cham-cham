// Ŭnicode please 
#pragma once

#include "cbes_globals.h"
#include "message_handler.h"

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

	virtual void update(int ms) = 0;
	virtual void setUp() = 0;
	virtual void shutDown() = 0;
	void registerObject(int compId, Object *obj);
	Object *getGameObject(int compId) { return GameObjectList[compId]; }

	virtual int create();
	virtual void destroy(int compId);

	int remainPoolSize() const { return CompPool.size(); }

public:
	virtual void initMsgHandler() = 0;
	virtual void onMessage(int compId, const BaseMessage *msg);

protected:
	template<typename T, typename MsgT>
	void registerMsgFunc(T *instance, void (T::*mem_fn)(int, MsgT*)) {
		MsgHandler.RegisterMessageFunc(instance, mem_fn);
	}
	MessageHandler MsgHandler;


protected:
	std::vector<int> CompPool;
	std::vector<bool> ActiveList;
	std::vector<Object*> GameObjectList;
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

template<typename T, int Comp>
struct ComponentListTypeHolder {
	enum { kComp = Comp };
	typedef std::vector<T> list_type;

	static T *getComp(list_type &val, int idx) 
	{
		SR_ASSERT(idx >= 0 && idx < (int)val.size());
		return &val[idx];	
	}
};

template<typename T>
struct ComponentListTypeHolder<T, kCompNone> {
	enum { kComp = kCompNone };
	typedef std::vector<T*> list_type;

	static T *getComp(list_type &val, int idx) 
	{
		SR_ASSERT(idx >= 0 && idx < (int)val.size());
		return val[idx];	
	}
};


template<typename T>
class ObjectBasedComponentList : public BaseComponentList {
public:
	typedef T value_type;
	typedef ComponentListTypeHolder<T, T::kComp> ListTypeHolder;

	enum {
		kFamily = T::kFamily,
		kComp = T::kComp,
	};

	static_assert(std::is_base_of<BaseComponent, T>::value == 1, "support only BaseComponent");
	static_assert(kFamily > kFamilyNone && kFamily < cNumFamily, "not valid family code");
	static_assert(kComp >= kCompNone && kComp < cNumComp, "not valid comp code");

public:
	virtual comp_id_type getFamilyId() const { return kFamily; }
	virtual comp_id_type getComponentId() const { return kComp; }

public:
	ObjectBasedComponentList(int poolSize=kDefaultPoolSize) 
		: BaseComponentList(poolSize) { CompList.resize(poolSize); }
	
	void clear();
	void update(int ms);
	T *getComp(int compId) { return ListTypeHolder::getComp(CompList, compId); }
	virtual void destroy(int compId) = 0;

	// 메세지 처리가 필요한 컴포넌트 리스트의 경우
	// 해당 클래스에 관련된것을 상속한 다음에 메세지 관련 함수를 구현하면 된다
	// CompHealthList를 참고
	virtual void initMsgHandler() { /* TODO implement */ }

	virtual void setUp() { initMsgHandler(); }
	virtual void shutDown() { }

protected:
	typename ListTypeHolder::list_type CompList;
};

template<typename T>
class SimpleComponentList : public ObjectBasedComponentList<T> {
public:
	typedef ObjectBasedComponentList<T> Parent;
public:
	SimpleComponentList(int poolSize=kDefaultPoolSize) : Parent(poolSize) {}
	virtual ~SimpleComponentList() { Parent::clear(); }
	
	int create();
	virtual void destroy(int compId);
};


/*
상속으로 구성되는 컴포넌트의 경우는 Component Type를 특정값 하나로 지정할수없다
그래서 이 경우는 kCompNone을 사용해서 Family Type만 명확한거로 설정
*/
template<typename T>
class InheritanceComponentList : public ObjectBasedComponentList<T> {
public:
	typedef ObjectBasedComponentList<T> Parent;

public:
	InheritanceComponentList(int poolSize=kDefaultPoolSize) : Parent(poolSize) {}
	virtual ~InheritanceComponentList() { Parent::clear(); }
	
	int add(T *obj);
	virtual void destroy(int compId);
};

/////////////////////////////////////////
// template implement
template<typename T>
void ObjectBasedComponentList<T>::clear()
{
	for(size_t i = 0 ; i < ActiveList.size() ; ++i) {
		int active = ActiveList[i];
		if(active == false) {
			continue;
		}
		destroy(i);
	}
}

template<typename T>
void ObjectBasedComponentList<T>::update(int ms)
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
int SimpleComponentList<T>::create()
{
	int compId = BaseComponentList::create();
	T &obj = Parent::CompList[compId];
	obj.setUp();
	return compId;
}

template<typename T>
void SimpleComponentList<T>::destroy(int compId)
{
	T &obj = Parent::CompList[compId];
	obj.shutDown();
	BaseComponentList::destroy(compId);
}


template<typename T>
int InheritanceComponentList<T>::add(T *obj)
{
	int compId = BaseComponentList::create();
	Parent::CompList[compId] = obj;
	obj->setUp();
	return compId;
}

template<typename T>
void InheritanceComponentList<T>::destroy(int compId)
{
	T *obj = Parent::CompList[compId];
	obj->shutDown();
	delete(obj);
	BaseComponentList::destroy(compId);
}