// Ŭnicode please 
#include "stdafx.h"
#include "sample_component.h"


CompHealthProxy::CompHealthProxy()
	: Active(false)
{
	std::fill(InitialHP.begin(), InitialHP.end(), nullptr);
	std::fill(CurrentHP.begin(), CurrentHP.end(), nullptr);
}

CompHealthList::CompHealthList(int poolSize)
	: Parent(poolSize)
{
	InitialHPList_Head.resize(poolSize);
	InitialHPList_Torso.resize(poolSize);
	CurrentHPList_Head.resize(poolSize);
	CurrentHPList_Torso.resize(poolSize);
}

int CompHealthList::create(const healt_value_t hp)
{
	int compId = Parent::create();
	CompHealthProxy comp = getComp(compId);
	for(int i = 0 ; i < cNumBodyParts ; ++i) {
		*comp.InitialHP[i] = hp;
		*comp.CurrentHP[i] = hp;
	}
	return compId;
}

void CompHealthList::initMsgHandler()
{
	registerMsgFunc(this, &CompHealthList::onDestroyMessage);
}
void CompHealthList::onDestroyMessage(int compId, DestroyMessage *msg)
{
	//component message 사용 예제 코드
	CompHealthProxy comp = getComp(compId);
	if(comp.Active == false) {
		return;
	}

	for(int i = 0 ; i < cNumBodyParts ; ++i) {
		*comp.CurrentHP[i] = 0;
	}
}

void CompHealthList::setUp()
{
	initMsgHandler();
}

void CompHealthList::shutDown()
{
}

void CompHealthList::update(int ms)
{
}

CompHealthProxy CompHealthList::getComp(int compId) const
{
	CompHealthProxy comp;
	if(ActiveList[compId] == true) {
		comp.Active = true;
		comp.InitialHP[head] = const_cast<int*>(&InitialHPList_Head[compId]);
		comp.InitialHP[torso] = const_cast<int*>(&InitialHPList_Torso[compId]);
		comp.CurrentHP[head] = const_cast<int*>(&CurrentHPList_Head[compId]);
		comp.CurrentHP[torso] = const_cast<int*>(&CurrentHPList_Torso[compId]);
	} else {
		comp.Active = false;
	}
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
