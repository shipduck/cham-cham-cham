// Ŭnicode please 
#include "stdafx.h"
#include "cbes/game_object.h"

TEST(CompHealthList, test)
{
	CompHealthList compList;
	int compIdA = compList.create(10);
	int compIdB = compList.create(20);

	EXPECT_EQ(10, compList.getInitialHealthAt(compIdA, CompHealthList::head));
	EXPECT_EQ(20, compList.getInitialHealthAt(compIdB, CompHealthList::torso));

	compList.setHealthAt(compIdA, CompHealthList::head, 5);
	EXPECT_EQ(true, compList.isWounded(compIdA));
	EXPECT_EQ(false, compList.isWounded(compIdB));
}

TEST(SimpleComponentList, test)
{
	SimpleComponentList<CompHealth> compList;
	int compIdA = compList.create();
	compList.getComp(compIdA)->init(10);

	int compIdB = compList.create();
	CompHealth *compB = compList.getComp(compIdB);
	compB->init(20);

	compList.destroy(compIdA);
	compList.destroy(compIdB);
}

TEST(InheritanceComponentList, test)
{
	InheritanceComponentList<ICompVisual> compList;
	CompVisualSphere *sphereA = new CompVisualSphere();
	sphereA->init(10);

	CompVisualSphere *sphereB = new CompVisualSphere();
	sphereB->init(20);

	int sphereIdA = compList.add(sphereA);
	int sphereIdB = compList.add(sphereB);

	compList.destroy(sphereIdA);
}

TEST(World, test)
{
	World world;
	
	Object *a = world.create();
	Object *b = world.create();
	EXPECT_EQ(2, world.size());

	world.remove(b);
	EXPECT_EQ(1, world.size());

	world.remove(a);
	EXPECT_EQ(0, world.size());
}

TEST(Object, createComponent)
{
	World world;
	Object *obj = world.create();

	EXPECT_EQ(false, obj->hasComponent(kFamilySimpleHP));
	EXPECT_EQ(false, obj->hasComponent(kFamilyHealth));
	EXPECT_EQ(false, obj->hasComponent(kFamilyVisual));

	{
		SimpleHPCompCreator creator(obj);
		int compId = creator.create(10);
	}

	EXPECT_EQ(true, obj->hasComponent(kFamilySimpleHP));

	{
		HealthCompCreator creator(obj);
		int compId = creator.create(10);
	}

	EXPECT_EQ(true, obj->hasComponent(kFamilyHealth));

	{
		VisualCompCreator creator(obj);
		int compId = creator.createSphere(5);
	}

	EXPECT_EQ(true, obj->hasComponent(kFamilyVisual));
}

TEST(Object, removeComponent)
{
	World world;
	Object *obj = world.create();

	SimpleHPCompCreator simpleHPCreator(obj);
	simpleHPCreator.create(10);
	HealthCompCreator healthCreator(obj);
	healthCreator.create(10);
	VisualCompCreator visualCreator(obj);
	visualCreator.createSphere(5);

	std::array<int, 3> familyTypeList = {
		kFamilySimpleHP, kFamilyHealth, kFamilyVisual
	};

	for(auto familyType : familyTypeList) {
		EXPECT_EQ(true, obj->hasComponent(familyType));
		obj->removeComponent(familyType);
		EXPECT_EQ(false, obj->hasComponent(familyType));
	}
}