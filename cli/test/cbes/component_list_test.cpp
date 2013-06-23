// Ŭnicode please 
#include "stdafx.h"
#include "cbes/component_list.h"
#include "cbes/sample_component.h"

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

TEST(BaseComponentList, getFamilyId_getCompoonentId)
{
	{
		typedef CompHealthList CompListType;
		CompListType compList;
		EXPECT_EQ(kFamilyHealth, CompListType::kFamily);
		EXPECT_EQ(kFamilyHealth, compList.getFamilyId());
		EXPECT_EQ(kCompHealth, (int)CompListType::kComp);
		EXPECT_EQ(kCompHealth, compList.getComponentId());
	}

	{
		typedef SimpleComponentList<CompHealth> CompListType;
		CompListType compList;
		EXPECT_EQ(kFamilySimpleHP, CompListType::kFamily);
		EXPECT_EQ(kFamilySimpleHP, compList.getFamilyId());
		EXPECT_EQ(kCompHealth, (int)CompListType::kComp);
		EXPECT_EQ(kCompHealth, compList.getComponentId());
	}

	{
		typedef InheritanceComponentList<ICompVisual> CompListType;
		CompListType compList;
		EXPECT_EQ(kFamilyVisual, CompListType::kFamily);
		EXPECT_EQ(kFamilyVisual, compList.getFamilyId());
		EXPECT_EQ(kCompNone, (int)CompListType::kComp);
		EXPECT_EQ(kCompNone, compList.getComponentId());
	}
}