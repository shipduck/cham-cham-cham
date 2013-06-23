// Ŭnicode please 
#include "stdafx.h"
#include "cbes/component_list.h"
#include "cbes/sample_component.h"

using namespace std;

TEST(BaseComponentList, getFamilyId_getCompoonentId)
{
	{
		typedef CompHealthList CompListType;
		CompListType compList;
		const int family = kFamilyHealth;
		const int comp = kCompHealth;
		EXPECT_EQ(family, CompListType::kFamily);
		EXPECT_EQ(family, compList.getFamilyId());
		EXPECT_EQ(comp, (int)CompListType::kComp);
		EXPECT_EQ(comp, compList.getComponentId());
	}

	{
		typedef SimpleComponentList<CompHealth> CompListType;
		CompListType compList;
		const int family = kFamilySimpleHP;
		const int comp = kCompHealth;
		EXPECT_EQ(family, CompListType::kFamily);
		EXPECT_EQ(family, compList.getFamilyId());
		EXPECT_EQ(comp, (int)CompListType::kComp);
		EXPECT_EQ(comp, compList.getComponentId());
	}

	{
		typedef InheritanceComponentList<ICompVisual> CompListType;
		CompListType compList;
		const int family = kFamilyVisual;
		const int comp = kCompNone;
		EXPECT_EQ(family, CompListType::kFamily);
		EXPECT_EQ(family, compList.getFamilyId());
		EXPECT_EQ(comp, (int)CompListType::kComp);
		EXPECT_EQ(comp, compList.getComponentId());
	}
}

TEST(BaseComponentList, remainPoolSize)
{
	{
		CompHealthList compList;
		int origPoolSize = compList.remainPoolSize();

		int compIdA = compList.create(10);
		EXPECT_EQ(origPoolSize - 1, compList.remainPoolSize());

		compList.destroy(compIdA);
		EXPECT_EQ(origPoolSize, compList.remainPoolSize());
	}
	{
		SimpleComponentList<CompHealth> compList;
		int origPoolSize = compList.remainPoolSize();

		int compIdA = compList.create();
		compList.getComp(compIdA)->init(10);
		EXPECT_EQ(origPoolSize - 1, compList.remainPoolSize());

		compList.destroy(compIdA);
		EXPECT_EQ(origPoolSize, compList.remainPoolSize());

	}
	{
		InheritanceComponentList<ICompVisual> compList;
		int origPoolSize = compList.remainPoolSize();

		CompVisualSphere *sphereA = new CompVisualSphere();
		sphereA->init(10);
		int sphereIdA = compList.add(sphereA);
		EXPECT_EQ(origPoolSize - 1, compList.remainPoolSize());

		compList.destroy(sphereIdA);
		EXPECT_EQ(origPoolSize, compList.remainPoolSize());
	}
}

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

TEST(ComponentListTypeHolder, list_type)
{
	{
		typedef ICompVisual CompType;
		typedef ComponentListTypeHolder<CompType, CompType::kComp> HolderType;
		typedef vector<CompType*> ExpectType;

		static_assert(HolderType::kComp == kCompNone, "not valid component type");
		static_assert(std::is_same<HolderType::list_type, ExpectType>::value == 1, "expected not same");
	}
	{
		typedef CompHealth CompType;
		typedef ComponentListTypeHolder<CompType, CompType::kComp> HolderType;
		typedef vector<CompType> ExpectType;

		static_assert(HolderType::kComp != kCompNone, "not valid component type");
		static_assert(std::is_same<HolderType::list_type, ExpectType>::value == 1, "expected not same");
	}
}