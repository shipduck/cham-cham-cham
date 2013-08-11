// Ŭnicode please 
#include "stdafx.h"
#include "fps_counter.h"
#include "base/lib.h"

#include "irr/debug_draw_manager.h"
#include "cvars/CVar.h"
#include "cvar_key.h"

using namespace std;
using namespace irr;

FPSCounter::FPSCounter()
	: visible(CVarUtils::GetCVarRef<int>(CVAR_ENGINE_DISPLAY_SHOW_FPS))
{
}

FPSCounter::~FPSCounter()
{
}

s32 FPSCounter::getFPS() const
{
	int fps = Lib::driver->getFPS();
	return fps;
}

u32 FPSCounter::getPrimitive() const
{
	const int param = 0;
	int retval = Lib::driver->getPrimitiveCountDrawn(param);
	return retval;
	
}

u32 FPSCounter::getPrimitiveAverage() const
{
	const int param = 1;
	int retval = Lib::driver->getPrimitiveCountDrawn(param);
	return retval;
}

u32 FPSCounter::getPrimitiveTotal() const
{
	const int param = 2;
	int retval = Lib::driver->getPrimitiveCountDrawn(param);
	return retval;
}

void FPSCounter::draw()
{
	if(visible) {
		drawText(0, 0);
		drawCaption();
	}
}

void FPSCounter::drawText(int baseX, int baseY)
{
	int fps = getFPS();
	int primitive = getPrimitive();
	int primitiveAverage = getPrimitiveAverage();
	int primitiveTotal = getPrimitiveTotal();

	video::SColor white(255, 255, 255, 255);
	
	using boost::wformat;
	auto fpsMsg = (wformat(L"FPS : %d") % fps).str();
	auto primitiveMsg = (wformat(L"Primitive : %d") % primitive).str();
	auto primitiveAvgMsg = (wformat(L"PrimitiveAvg : %d") % primitiveAverage).str();
	auto primitiveTotalMsg = (wformat(L"PrimitiveTotal : %d") % primitiveTotal).str();

	const int lineHeight = DebugDrawManager::kFont14Height;
	g_debugDrawMgr->addString(core::vector2di(baseX, baseY + lineHeight * 0), fpsMsg, white);
	g_debugDrawMgr->addString(core::vector2di(baseX, baseY + lineHeight * 1), primitiveMsg, white);
	g_debugDrawMgr->addString(core::vector2di(baseX, baseY + lineHeight * 2), primitiveAvgMsg, white);
	g_debugDrawMgr->addString(core::vector2di(baseX, baseY + lineHeight * 3), primitiveTotalMsg, white);
}
void FPSCounter::drawCaption()
{
	int fps = getFPS();
	core::stringw str = "FPS:";
	str += fps;
	Lib::device->setWindowCaption(str.c_str());
}