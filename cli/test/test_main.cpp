// Ŭnicode please 
#include "stdafx.h"

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	printf("Press Any Key + Enter...\n");
	getchar();
	return 0;	
}