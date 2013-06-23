// Ŭnicode please 
#include "stdafx.h"
#include "cbes/object.h"
#include "cbes/component_list.h"
#include "cbes/world.h"

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