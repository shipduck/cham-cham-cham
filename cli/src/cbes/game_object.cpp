// Ŭnicode please 
#include "stdafx.h"
#include "game_object.h"

using namespace std;

ComponentList::ComponentList(int poolSize)
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
int ComponentList::create()
{
	SR_ASSERT(CompPool.empty() == false);
	int compId = CompPool.back();
	CompPool.pop_back();
	ActiveList[compId] = 1;
	return compId;
}

void ComponentList::destroy(int compId)
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
	: ComponentList(poolSize)
{
	InitialHPList_Head.resize(poolSize);
	InitialHPList_Torso.resize(poolSize);
	CurrentHPList_Head.resize(poolSize);
	CurrentHPList_Torso.resize(poolSize);
}

int CompHealthList::create(const healt_value_t hp)
{
	int compId = ComponentList::create();
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