// Ŭnicode please
#include "stdafx.h"
#include "util/debug_draw_manager.h"

using namespace Loki;
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

TEST_F(DebugDrawManagerTest, updateAll) 
{
	/*
	DebugDrawManager mgr;
	EXPECT_EQ(0, mgr.size());

	SColor color;
	//mgr.addCross(vector2di(0, 0), color, 1);	// without duration
	//mgr.addCross(vector2di(0, 0), color, 1, 1000);	// with duration
	mgr.addLine(vector3df(0, 0, 0), vector3df(0, 0, 0), color, 1);	// without duration
	mgr.addLine(vector3df(0, 0, 0), vector3df(0, 0, 0), color, 1, 1000);	// with duration
	EXPECT_EQ(2, mgr.size());

	mgr.updateAll(500);
	EXPECT_EQ(1, mgr.size());

	mgr.updateAll(1000);
	EXPECT_EQ(0, mgr.size());

	mgr.updateAll(1000);
	EXPECT_EQ(0, mgr.size());
	*/
}

TEST_F(DebugDrawManagerTest, getCmdList)
{
	DebugDrawManager mgr;
	//auto line2dList = mgr.getList(Type2Type<DebugDraw_Line2D>());
	//auto cross2dList = mgr.getList<DebugDraw_Cross2D>();
}