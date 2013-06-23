// Ŭnicode please 
#include "stdafx.h"
#include "object.h"
#include "world.h"
#include "component_list.h"
#include "sample_component.h"

using namespace std;


Object::Object(World *world)
	: WorldPtr(world), id(world->NextId())
{
	std::fill(CompIdList.begin(), CompIdList.end(), -1);
}
Object::~Object()
{
	clearComponents();
}
BaseComponentList *Object::getCompList(comp_id_type familyType)
{
	return WorldPtr->getCompList(familyType);
}
void Object::setComponent(comp_id_type familyType, int compId)
{
	SR_ASSERT(compId >= 0);
	CompIdList[familyType] = compId;

	//component가 object에 바로 접근할 수 있도록 연결
	WorldPtr->getCompList(familyType)->registerObject(compId, this);
}
bool Object::hasComponent(comp_id_type familyType) const
{
	if(CompIdList[familyType] < 0) {
		return false;
	}
	return true;
}

bool Object::removeComponent(comp_id_type familyType)
{
	if(CompIdList[familyType] < 0) {
		return false;
	}
	auto compList = getCompList(familyType);
	compList->destroy(CompIdList[familyType]);
	CompIdList[familyType] = -1;
	return true;
}

void Object::clearComponents()
{
	for(int familyType = 0 ; familyType < cNumFamily ; ++familyType) {
		int compId = CompIdList[familyType];
		if(compId < 0) {
			continue;
		}
		BaseComponentList *compList = WorldPtr->getCompList(familyType);
		compList->destroy(compId);
		CompIdList[familyType] = -1;
	}
}



int SimpleHPCompCreator::create(int hp)
{
	typedef SimpleComponentList<CompHealth> CompListType;
	CompListType *compList = getCompList<CompListType>();

	int compId = compList->create();
	compList->getComp(compId)->init(hp);

	setComponent(compId);
	return compId;
}

int HealthCompCreator::create(int hp)
{
	typedef CompHealthList CompListType;
	CompListType *compList = getCompList<CompListType>();

	int compId = compList->create(hp);

	setComponent(compId);
	return compId;
}

int VisualCompCreator::createSphere(float radius)
{
	typedef InheritanceComponentList<ICompVisual> CompListType;
	CompListType *compList = getCompList<CompListType>();

	CompVisualSphere *sphere = new CompVisualSphere();
	sphere->init(radius);

	int compId = compList->add(sphere);

	setComponent(compId);
	return compId;

}