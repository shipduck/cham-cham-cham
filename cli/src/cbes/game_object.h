// Ŭnicode please 
#pragma once

class Object;
class World;
typedef int comp_id_type;
typedef int obj_id_type;

enum {
	kFamilyNone = -1,
	kFamilyHealth = 0,
	kFamilySimpleHP,
	kFamilyVisual,
	cNumFamily,
};

enum {
	kCompNone = -1,
	kCompHealth = 0,
	kCompVisualSphere,
	cNumComp,
};

class BaseComponent {
public:
	virtual void setUp() = 0;
	virtual void shutDown() = 0;
	virtual void update(int ms) = 0;

	virtual comp_id_type getComponentId() const = 0;
	virtual comp_id_type getFamilyId() const = 0;
};

class BaseComponentList {
public:
	BaseComponentList(int poolSize=32);
	virtual ~BaseComponentList() {}

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

class CompHealthList : public BaseComponentList {
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
class SimpleComponentList : public BaseComponentList {
public:
	typedef T value_type;

public:
	SimpleComponentList(int poolSize=32)
		: BaseComponentList(poolSize)
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
		int compId = BaseComponentList::create();
		T &obj = CompList[compId];
		obj.setUp();
		return compId;
	}

	void destroy(int compId)
	{
		T &obj = CompList[compId];
		obj.shutDown();
		BaseComponentList::destroy(compId);
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
class InheritanceComponentList : public BaseComponentList {
public:
	typedef T value_type;

public:
	InheritanceComponentList(int poolSize=32)
		: BaseComponentList(poolSize)
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
		int compId = BaseComponentList::create();
		CompList[compId] = obj;
		obj->setUp();
		return compId;
	}

	void destroy(int compId)
	{
		T *obj = CompList[compId];
		obj->shutDown();
		delete(obj);
		BaseComponentList::destroy(compId);
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

class World {
public:
	World();
	~World();

	Object *create();
	void remove(Object *obj);
	int size() const;

	BaseComponentList *getCompList(comp_id_type familyType);
	obj_id_type NextId();

private:
	// component list를 적절히 선언
	CompHealthList HealthList;
	SimpleComponentList<CompHealth> SimpleHealthList;
	InheritanceComponentList<ICompVisual> VisualList;

	std::unordered_map<int, Object*> ObjectMap;

	int nextId;
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