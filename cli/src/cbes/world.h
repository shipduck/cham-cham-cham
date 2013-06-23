// Ŭnicode please 
#pragma once

#include "cbes_globals.h"

class World {
public:
	World();
	~World();

	Object *create();
	void remove(Object *obj);
	int size() const;

	BaseComponentList *getCompList(comp_id_type familyType);
	void setCompList(comp_id_type familyType, BaseComponentList *compList);
	obj_id_type NextId();

private:
	std::array<BaseComponentList*, cNumFamily> CompListMap;
	std::unordered_map<int, Object*> ObjectMap;

	int nextId;
};

