// Ŭnicode please 
#include "stdafx.h"
#include "test_main.h"

using namespace irr;
using namespace core;
using namespace video;

irr::IrrlichtDevice *g_device = nullptr;

int main(int argc, char *argv[])
{
	int width = 320;
	int height = 240;
	g_device = createDevice(EDT_OPENGL, dimension2d<u32>(width, height), 16, false, false, true, 0);
	if(!g_device) {
		printf("Cannot create device!\n");
		return 1;
	}

	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	g_device->drop();
	g_device = nullptr;

	printf("Press Any Key + Enter...\n");
	getchar();
	return 0;	
}