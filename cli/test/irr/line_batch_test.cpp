// Ŭnicode please 
#include "stdafx.h"
#include "irr/line_batch.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace std;

SColor red(255, 255, 0, 0);
SColor green(255, 0, 255, 0);

TEST(LineBatch, add)
{
	LineBatch batch;
	batch.add(vector2di(1, 2), vector2di(3, 4), red);
	batch.add(vector3df(3, 2, 1), vector3df(5, 4, 3), green);

	EXPECT_EQ(4, batch.vertexList.size());
	EXPECT_EQ(4, batch.indexList.size());

	EXPECT_EQ(vector3df(1, 2, 0), batch.vertexList[0].Pos);
	EXPECT_EQ(vector3df(3, 4, 0), batch.vertexList[1].Pos);
	EXPECT_EQ(red, batch.vertexList[0].Color);
	EXPECT_EQ(red, batch.vertexList[1].Color);

	EXPECT_EQ(vector3df(3, 2, 1), batch.vertexList[2].Pos);
	EXPECT_EQ(vector3df(5, 4, 3), batch.vertexList[3].Pos);
	EXPECT_EQ(green, batch.vertexList[2].Color);
	EXPECT_EQ(green, batch.vertexList[3].Color);
}