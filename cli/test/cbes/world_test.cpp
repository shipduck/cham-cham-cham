// Ŭnicode please 
#include "stdafx.h"
#include "cbes/world.h"
#include "cbes/object.h"

TEST(World, create_and_remove)
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
