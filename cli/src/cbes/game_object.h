// Ŭnicode please 
#pragma once

class Object;
typedef int comp_id_type;

const int kFamilyNone = 0;
const int kFamilyHealth = 1;
const int kFamilyVisual = 2;

const int kCompNone = 0;
const int kCompHealth = 1;
const int kCompVisualSphere = 2;


class ComponentList {
public:
	ComponentList(int poolSize=32);
	virtual ~ComponentList() {}

	virtual const comp_id_type getFamilyId() const = 0;

	virtual void update(int ms) {}

	int create();
	void destroy(int compId);

protected:
	std::vector<int> CompPool;
	std::vector<int> ActiveList;
};

class CompHealthProxy {
public:
	CompHealthProxy();
	int *Active;
	std::array<int*, 2> InitialHP;
	std::array<int*, 2> CurrentHP;
};

class CompHealthList : public ComponentList {
public:
	typedef int healt_value_t;
	enum bodyPart_e {
		head = 0, torso, cNumBodyParts 
	};

public:
	CompHealthList(int poolSize=32);
	
	int create(const healt_value_t hp);

	virtual const comp_id_type getComponentId() const { return kCompHealth; }
	virtual const comp_id_type getFamilyId() const { return kFamilyHealth; }

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

class BaseComponent {
public:
	virtual void setUp() = 0;
	virtual void shutDown() = 0;
	virtual void update(int ms) = 0;

	virtual comp_id_type getComponentId() const = 0;
	virtual comp_id_type getFamilyId() const = 0;
};

template<int FamilyCode, int CompCode>
class BaseComponentT : public BaseComponent {
public:
	enum {
		kFamily = FamilyCode,
		kComp = CompCode
	};
public:
	virtual comp_id_type getComponentId() const { return CompCode; }
	virtual comp_id_type getFamilyId() const { return FamilyCode; }
};

class CompHealth : public BaseComponentT<kFamilyHealth, kCompHealth> {
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
	};
public:
	virtual void render() const = 0;

	virtual comp_id_type getFamilyId() const { return kFamily; }
};

class CompVisualSphere : public ICompVisual {
public:
	virtual void setUp() {}
	virtual void shutDown() {}
	virtual void update(int ms);

	virtual comp_id_type getComponentId() const { return kCompVisualSphere; }

	void init(float radius);

	virtual void render() const;

	const float getRadius() const { return radius; }
	void setRadius(const float r) { radius = r; }
private:
	float radius;
};

template<typename T>
class SimpleComponentList : public ComponentList {
public:
	typedef T value_type;

public:
	SimpleComponentList(int poolSize=32)
		: ComponentList(poolSize)
	{
		CompList.resize(poolSize);
	}

	virtual const comp_id_type getFamilyId() const { return T::kFamily; }
	
	T *getComp(int compId)
	{
		SR_ASSERT(compId >= 0 && compId < (int)CompList.size());
		return &CompList[compId];
	}

	int create()
	{
		int compId = ComponentList::create();
		T &obj = CompList[compId];
		obj.setUp();
		return compId;
	}

	void destroy(int compId)
	{
		T &obj = CompList[compId];
		obj.shutDown();
		ComponentList::destroy(compId);
	}
	

	void update(int ms)
	{
		for(size_t i = 0 ; i < ActiveList.size() ; ++i) {
			int active = ActiveList[i];
			if(active == 0) {
				continue;
			}
			T *comp = getComp(i);
			comp->update(ms);
		}
	}

public:
	std::vector<T> CompList;
};

template<typename T>
class InheritanceComponentList : public ComponentList {
public:
	typedef T value_type;

public:
	InheritanceComponentList(int poolSize=32)
		: ComponentList(poolSize)
	{
		CompList.resize(poolSize);
	}
	~InheritanceComponentList() 
	{
		for(size_t i = 0 ; i < ActiveList.size() ; ++i) {
			int active = ActiveList[i];
			if(active == 0) {
				continue;
			}
			T *comp = getComp(i);
			delete(comp);
		}
	}

	virtual const comp_id_type getFamilyId() const { return T::kFamily; }

	T *getComp(int compId)
	{
		SR_ASSERT(compId >= 0 && compId < (int)CompList.size());
		return CompList[compId];
	}

	int add(T *obj)
	{
		int compId = ComponentList::create();
		CompList[compId] = obj;
		obj->setUp();
		return compId;
	}

	void destroy(int compId)
	{
		T *obj = CompList[compId];
		obj->shutDown();
		delete(obj);
		ComponentList::destroy(compId);
	}

	void update(int ms)
	{
		for(size_t i = 0 ; i < ActiveList.size() ; ++i) {
			int active = ActiveList[i];
			if(active == 0) {
				continue;
			}
			T *comp = getComp(i);
			comp->update(ms);
		}
	}

public:
	std::vector<T*> CompList;
};