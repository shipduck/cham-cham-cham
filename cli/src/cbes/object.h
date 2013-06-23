// Ŭnicode please 
#pragma once

#include "cbes_globals.h"

class Object {
public:
	Object(World *world);
	~Object();

	obj_id_type getId() const { return id; }

	void clearComponents();
	BaseComponentList *getCompList(comp_id_type familyType);

	void setComponent(comp_id_type familyType, int compId);
	bool hasComponent(comp_id_type familyType) const;
	bool removeComponent(comp_id_type familyType);

private:
	//존재하는 컴포넌트에 대한 comp id를 전부 저장할수 있도록한다
	//뭔가 막장같지만 이렇게 한 이유는
	//O(1)의 포스를 느끼기 위해서
	std::array<int, cNumFamily> CompIdList;

	World *WorldPtr;

	int id;
};

template<int FamilyCode>
class ComponentCreator {
public:
	enum { kFamily = FamilyCode };
public:
	ComponentCreator(Object *obj) : Obj(obj) {}
protected:
	Object *Obj;

	template<typename T>
	T *getCompList()
	{
		BaseComponentList *baseCompList = Obj->getCompList(kFamily);
		T *compList = static_cast<T*>(baseCompList);
		return compList;
	}
	void setComponent(int compId)
	{
		Obj->setComponent(kFamily, compId);
	}
};

class SimpleHPCompCreator : public ComponentCreator<kFamilySimpleHP> {
public:
	SimpleHPCompCreator(Object *obj) : ComponentCreator(obj) {}
	int create(int hp);
};

class HealthCompCreator : public ComponentCreator<kFamilyHealth> {
public:
	HealthCompCreator(Object *obj) : ComponentCreator(obj) {}
	int create(int hp);
};

class VisualCompCreator : public ComponentCreator<kFamilyVisual> {
public:
	VisualCompCreator(Object *obj) : ComponentCreator(obj) {}
	int createSphere(float radius);
};