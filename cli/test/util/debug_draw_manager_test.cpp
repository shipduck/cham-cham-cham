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

TEST(swapVectorElement, test)
{
	DebugDrawListMixin_3D vec;
	vec.depthEnableList.push_back(false);
	vec.depthEnableList.push_back(true);

	swapVectorElement(vec.depthEnableList, 0, 1);
	EXPECT_EQ(true, vec.depthEnableList[0]);
	EXPECT_EQ(false, vec.depthEnableList[1]);
}

TEST_F(DebugDrawManagerTest, updateAll) 
{
	DebugDrawManager mgr;
	EXPECT_EQ(0, mgr.size());

	SColor color;
	mgr.addCross(vector2di(0, 0), color, 1);	// without duration
	mgr.addCross(vector2di(0, 0), color, 1, 1000);	// with duration
	mgr.addLine(vector3df(0, 0, 0), vector3df(0, 0, 0), color, 1);	// without duration
	mgr.addLine(vector3df(0, 0, 0), vector3df(0, 0, 0), color, 1, 1000);	// with duration
	EXPECT_EQ(4, mgr.size());

	mgr.updateAll(500);
	EXPECT_EQ(2, mgr.size());

	mgr.updateAll(1000);
	EXPECT_EQ(0, mgr.size());

	mgr.updateAll(1000);
	EXPECT_EQ(0, mgr.size());
}

TEST_F(DebugDrawManagerTest, updateAll_complexDuration)
{
	DebugDrawManager mgr;
	EXPECT_EQ(0, mgr.size());

	SColor color;
	mgr.addLine(vector3df(0, 0, 0), vector3df(0, 0, 0), color, 1, 1000);
	mgr.addLine(vector3df(0, 0, 0), vector3df(0, 0, 0), color, 2, 3000);
	mgr.addLine(vector3df(0, 0, 0), vector3df(0, 0, 0), color, 3, 2000);
	mgr.addLine(vector3df(0, 0, 0), vector3df(0, 0, 0), color, 4, 4000);
	EXPECT_EQ(4, mgr.size());

	const DebugDrawList_Line3D &durationDrawList = Loki::Field<DebugDrawList_Line3D>(mgr).durationDrawList;
	const std::vector<int> &durationList = Loki::Field<DebugDrawList_Line3D>(mgr).durationList;

	mgr.updateAll(500);
	EXPECT_EQ(4, mgr.size());
	EXPECT_EQ(4, durationDrawList.size());
	EXPECT_EQ(4, durationList.size());
	{
		auto &vec = static_cast<DebugDrawListMixin_Scale<float>>(durationDrawList);
		EXPECT_EQ(4, vec.size());
		EXPECT_EQ(4, vec.scaleList[0]);
		EXPECT_EQ(2, vec.scaleList[1]);
		EXPECT_EQ(3, vec.scaleList[2]);
		EXPECT_EQ(1, vec.scaleList[3]);
	}

	mgr.updateAll(1000);
	EXPECT_EQ(3, mgr.size());
	EXPECT_EQ(3, durationDrawList.size());
	EXPECT_EQ(3, durationList.size());
	{
		auto &vec = static_cast<DebugDrawListMixin_Scale<float>>(durationDrawList);
		EXPECT_EQ(3, vec.size());
		EXPECT_EQ(4, vec.scaleList[0]);
		EXPECT_EQ(2, vec.scaleList[1]);
		EXPECT_EQ(3, vec.scaleList[2]);
	}

	mgr.updateAll(1000);
	EXPECT_EQ(2, mgr.size());
	EXPECT_EQ(2, durationDrawList.size());
	EXPECT_EQ(2, durationList.size());
	{
		auto &vec = static_cast<DebugDrawListMixin_Scale<float>>(durationDrawList);
		EXPECT_EQ(2, vec.size());
		EXPECT_EQ(4, vec.scaleList[0]);
		EXPECT_EQ(2, vec.scaleList[1]);
	}

	mgr.updateAll(1000);
	EXPECT_EQ(1, mgr.size());
	EXPECT_EQ(1, durationDrawList.size());
	EXPECT_EQ(1, durationList.size());
	{
		auto &vec = static_cast<DebugDrawListMixin_Scale<float>>(durationDrawList);
		EXPECT_EQ(1, vec.size());
		EXPECT_EQ(4, vec.scaleList[0]);
	}

	mgr.updateAll(1000);
	EXPECT_EQ(0, mgr.size());
	EXPECT_EQ(0, durationDrawList.size());
	EXPECT_EQ(0, durationList.size());
	{
		auto &vec = static_cast<DebugDrawListMixin_Scale<float>>(durationDrawList);
		EXPECT_EQ(0, vec.size());
	}
}


TEST_F(DebugDrawManagerTest, getCmdList)
{
	DebugDrawManager mgr;
	const auto &line3d = Loki::Field<DebugDrawList_Line3D>(mgr);
	const auto &line2d = Loki::Field<DebugDrawList_Line2D>(mgr);
}
