// Ŭnicode please 
#include "stdafx.h"
#include "game_object.h"

using namespace std;

BaseComponentList::BaseComponentList(int poolSize)
{
	ActiveList.resize(poolSize);
	std::fill(ActiveList.begin(), ActiveList.end(), false);

	//pool의 내용을 채우기. 뒤에서부터 뽑아쓰는게 적절할테니까
	//N-1~0순서로 집어넣기
	CompPool.resize(poolSize);
	for(int i = 0 ; i < poolSize ; ++i) {
		CompPool[i] = poolSize - 1 - i;
	}
	SR_ASSERT(CompPool.back() == 0);
}
int BaseComponentList::create()
{
	SR_ASSERT(CompPool.empty() == false);
	int compId = CompPool.back();
	CompPool.pop_back();
	ActiveList[compId] = 1;
	return compId;
}

void BaseComponentList::destroy(int compId)
{
	SR_ASSERT(compId >= 0 && compId < (int)ActiveList.size());
	CompPool.push_back(compId);
	ActiveList[compId] = 0;
}

CompHealthProxy::CompHealthProxy()
	: Active(nullptr)
{
	std::fill(InitialHP.begin(), InitialHP.end(), nullptr);
	std::fill(CurrentHP.begin(), CurrentHP.end(), nullptr);
}

CompHealthList::CompHealthList(int poolSize)
	: BaseComponentList(poolSize)
{
	InitialHPList_Head.resize(poolSize);
	InitialHPList_Torso.resize(poolSize);
	CurrentHPList_Head.resize(poolSize);
	CurrentHPList_Torso.resize(poolSize);
}

int CompHealthList::create(const healt_value_t hp)
{
	int compId = BaseComponentList::create();
	CompHealthProxy comp = getComp(compId);
	for(int i = 0 ; i < cNumBodyParts ; ++i) {
		*comp.InitialHP[i] = hp;
		*comp.CurrentHP[i] = hp;
	}
	return compId;
}



void CompHealthList::update(int ms)
{
}

CompHealthProxy CompHealthList::getComp(int compId) const
{
	CompHealthProxy comp;
	comp.Active = const_cast<int*>(&ActiveList[compId]);
	comp.InitialHP[head] = const_cast<int*>(&InitialHPList_Head[compId]);
	comp.InitialHP[torso] = const_cast<int*>(&InitialHPList_Torso[compId]);
	comp.CurrentHP[head] = const_cast<int*>(&CurrentHPList_Head[compId]);
	comp.CurrentHP[torso] = const_cast<int*>(&CurrentHPList_Torso[compId]);
	return comp;
}

CompHealthList::healt_value_t CompHealthList::getInitialHealthAt(int compId, const bodyPart_e part) const
{
	CompHealthProxy comp = getComp(compId);
	return *comp.InitialHP[part];
}

void CompHealthList::setInitialHealthAt(int compId, const bodyPart_e part, const healt_value_t hp)
{
	CompHealthProxy comp = getComp(compId);
	*comp.InitialHP[part] = hp;
}

CompHealthList::healt_value_t CompHealthList::getHealthAt(int compId, const bodyPart_e part) const 
{
	CompHealthProxy comp = getComp(compId);
	return *comp.CurrentHP[part];
}

void CompHealthList::setHealthAt(int compId, const bodyPart_e part, const healt_value_t hp)
{
	CompHealthProxy comp = getComp(compId);
	*comp.CurrentHP[part] = hp;
}

bool CompHealthList::isWounded(int compId) const
{
	CompHealthProxy comp = getComp(compId);
	for(int i = 0 ; i < cNumBodyParts ; ++i) {
		if(*comp.InitialHP[i] != *comp.CurrentHP[i]) {
			return true;
		}
	}
	return false;
}

void CompHealthList::reset(int compId)
{
	CompHealthProxy comp = getComp(compId);
	for(int i = 0 ; i < cNumBodyParts ; ++i) {
		comp.CurrentHP[i] = comp.InitialHP[i];
	}
}


void CompHealth::init(int hp)
{
	std::fill(InitialHP.begin(), InitialHP.end(), hp);
	std::fill(CurrentHP.begin(), CurrentHP.end(), hp);
}

CompHealth::healt_value_t CompHealth::getInitialHealthAt(const bodyPart_e part) const
{
	return InitialHP[part];
}
void CompHealth::setInitialHealthAt(const bodyPart_e part, const healt_value_t hp)
{
	InitialHP[part] = hp;
}

CompHealth::healt_value_t CompHealth::getHealthAt(const bodyPart_e part) const
{
	return CurrentHP[part];
}
void CompHealth::setHealthAt(const bodyPart_e part, const healt_value_t hp)
{
	CurrentHP[part] = hp;
}

bool CompHealth::isWounded() const
{
	for(int i = 0 ; i < cNumBodyParts ; ++i) {
		if(CurrentHP[i] != InitialHP[i]) {
			return true;
		}
	}
	return false;
}
void CompHealth::reset()
{
	for(int i = 0 ; i < cNumBodyParts ; ++i) {
		CurrentHP[i] = InitialHP[i];
	}
}

void CompVisualSphere::init(float radius)
{
	this->radius = radius;
}

void CompVisualSphere::render() const
{
	printf("render sphere with radius %f\n", radius);
}
void CompVisualSphere::update(int ms)
{
	render();
}

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
	CompIdList[familyType] = compId;
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

World::World()
	: nextId(1)
{
}
World::~World()
{
	auto it = ObjectMap.begin();
	auto endit = ObjectMap.end();
	for( ; it != endit ; ++it) {
		delete(it->second);
	}
}
BaseComponentList *World::getCompList(comp_id_type familyType)
{
	BaseComponentList *compList = nullptr;
	switch(familyType) {
	case kFamilyHealth:
		compList = &HealthList;
		break;
	case kFamilySimpleHP:
		compList = &SimpleHealthList;
		break;
	case kFamilyVisual:
		compList = &VisualList;
		break;
	default:
		SR_ASSERT(!"not valid");
		return nullptr;
	}
	SR_ASSERT(compList->getFamilyId() == familyType);
	return compList;
}

obj_id_type World::NextId()
{
	int val = nextId;
	nextId++;
	return val;
}

Object *World::create()
{
	Object *obj = new Object(this);
	ObjectMap[obj->getId()] = obj;	
	return obj;
}
void World::remove(Object *obj)
{
	auto found = ObjectMap.find(obj->getId());
	if(found != ObjectMap.end()) {
		ObjectMap.erase(found);
		delete(obj);
	}
}

int World::size() const
{
	return ObjectMap.size();
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