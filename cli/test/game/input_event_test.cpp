// Ŭnicode please
#include "stdafx.h"
#include "game/input_event.h"

TEST(LookEvent, merge)
{
	{
		LookEvent a;
		LookEvent b;
		b.horizontalRotation = 0.3f;
		b.verticalRotation = 0.5f;
		LookEvent c = a.merge(b);

		EXPECT_EQ(b.horizontalRotation, c.horizontalRotation);
		EXPECT_EQ(b.verticalRotation, c.verticalRotation);
	}
	{
		LookEvent a;
		a.horizontalRotation = 0.4f;
		LookEvent b;
		b.verticalRotation = 0.5f;
		LookEvent c = a.merge(b);

		EXPECT_EQ(a.horizontalRotation, c.horizontalRotation);
		EXPECT_EQ(b.verticalRotation, c.verticalRotation);
	}
}
TEST(MoveEvent, merge)
{
	{
		MoveEvent a;
		MoveEvent b;
		b.forwardBackward = 0.3f;
		b.leftRight = 0.5f;
		MoveEvent c = a.merge(b);

		EXPECT_EQ(b.forwardBackward, c.forwardBackward);
		EXPECT_EQ(b.leftRight, c.leftRight);
	}
	{
		MoveEvent a;
		a.forwardBackward = 0.4f;
		MoveEvent b;
		b.leftRight = 0.5f;
		MoveEvent c = a.merge(b);

		EXPECT_EQ(a.forwardBackward, c.forwardBackward);
		EXPECT_EQ(b.leftRight, c.leftRight);
	}
}