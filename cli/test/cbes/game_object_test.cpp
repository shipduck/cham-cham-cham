// Ŭnicode please 
#include "stdafx.h"
#include "cbes/game_object.h"

TEST(CompHealthList, test)
{
	CompHealthList compList;
	int compIdA = compList.create(10);
	int compIdB = compList.create(20);

	EXPECT_EQ(10, compList.getInitialHealthAt(compIdA, head));
	EXPECT_EQ(20, compList.getInitialHealthAt(compIdB, torso));

	compList.setHealthAt(compIdA, head, 5);
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