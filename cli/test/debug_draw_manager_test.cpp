// Ŭnicode please
#include "test_stdafx.h"
#include "debug_draw_manager.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;

class DebugDrawManagerTest : public ::testing::Test {
protected:
	virtual void SetUp()
	{	
	}
	virtual void TearDown()
	{
	}
};

TEST_F(DebugDrawManagerTest, Update) 
{
	DebugDrawManager2D mgr;
	EXPECT_EQ(0, mgr.count());

	SColor color;
	mgr.addCross(vector2df(0, 0), color, 1);	// without duration
	mgr.addCross(vector2df(0, 0), color, 1, 1000);	// with duration
	EXPECT_EQ(2, mgr.count());

	mgr.update(500);
	EXPECT_EQ(1, mgr.count());

	mgr.update(1000);
	EXPECT_EQ(0, mgr.count());

	mgr.update(1000);
	EXPECT_EQ(0, mgr.count());
}

