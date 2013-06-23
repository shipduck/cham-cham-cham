// Ŭnicode please 
#pragma once
#include "component_list.h"

class CompHealthProxy {
public:
	CompHealthProxy();
	int *Active;
	std::array<int*, 2> InitialHP;
	std::array<int*, 2> CurrentHP;
};

class CompHealthList : public BaseComponentListT<kFamilyHealth, kCompHealth> {
public:
	typedef int healt_value_t;
	typedef BaseComponentListT<kFamilyHealth, kCompHealth> Parent;
	enum bodyPart_e {
		head = 0, torso, cNumBodyParts,
		kFamily = kFamilyHealth,
	};

public:
	CompHealthList(int poolSize=32);
	
	int create(const healt_value_t hp);

	virtual void update(int ms);

	//Health interface
public:
	CompHealthProxy getComp(int compId) const;

	healt_value_t getInitialHealthAt(int compId, const bodyPart_e part) const;
	void setInitialHealthAt(int compId, const bodyPart_e part, const healt_value_t hp);

	healt_value_t getHealthAt(int compId, const bodyPart_e part) const;
	void setHealthAt(int compId, const bodyPart_e part, const healt_value_t hp);

	bool isWounded(int compId) const;
	void reset(int compId);

private:
	std::vector<int> InitialHPList_Head;
	std::vector<int> InitialHPList_Torso;

	std::vector<int> CurrentHPList_Head;
	std::vector<int> CurrentHPList_Torso;
};

class CompHealth : public BaseComponentT<kFamilySimpleHP, kCompHealth> {
public:
	typedef int healt_value_t;
	enum bodyPart_e {
		head = 0, torso, cNumBodyParts 
	};

public:
	virtual void setUp() {}
	virtual void shutDown() {}
	virtual void update(int ms) {}

	void init(int hp);

	healt_value_t getInitialHealthAt(const bodyPart_e part) const;
	void setInitialHealthAt(const bodyPart_e part, const healt_value_t hp);

	healt_value_t getHealthAt(const bodyPart_e part) const;
	void setHealthAt(const bodyPart_e part, const healt_value_t hp);

	bool isWounded() const;
	void reset();

private:
	std::array<int, 2> InitialHP;
	std::array<int, 2> CurrentHP;
};

class ICompVisual : public BaseComponent {
public:
	enum {
		kFamily = kFamilyVisual,
		kComp = kCompNone,
	};
public:
	virtual comp_id_type getFamilyId() const { return kFamily; }
	virtual comp_id_type getComponentId() const { return kComp; }
public:
	virtual void render() const = 0;
};

class CompVisualSphere : public ICompVisual {
public:
	virtual comp_id_type getComponentId() const { return kCompVisualSphere; }
public:
	virtual void setUp() {}
	virtual void shutDown() {}
	virtual void update(int ms);
	virtual void render() const;

public:
	void init(float radius);

	const float getRadius() const { return radius; }
	void setRadius(const float r) { radius = r; }
private:
	float radius;
};
