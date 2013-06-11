// Ŭnicode please 
#include <gtest/gtest.h>
#include <cstdio>

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	getchar();
	return 0;	
}