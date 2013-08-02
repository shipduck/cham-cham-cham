// Ŭnicode please 
#pragma once

#include "debug_draw_manager.h"

class LineBatch;

class DebugDrawer {
public:
	DebugDrawer();
	~DebugDrawer();

	void shutDown();
	void drawAll(irr::IrrlichtDevice *dev, const DebugDrawManager &mgr);

public:
	void drawList(const DrawAttributeList_Line2D &cmd);
	void drawList(const DrawAttributeList_Cross2D &cmd);
	void drawList(const DrawAttributeList_Circle2D &cmd);
	void drawList(const DrawAttributeList_String2D &cmd);
	void drawList(const DrawAttributeList_Line3D &cmd);
	void drawList(const DrawAttributeList_Cross3D &cmd);
	void drawList(const DrawAttributeList_Sphere3D &cmd);
	void drawList(const DrawAttributeList_Axis3D &cmd);
	void drawList(const DrawAttributeList_String3D &cmd);

private:
	irr::gui::IGUIFont *getDebugFont(irr::IrrlichtDevice *dev);

private:
	// thickness != 1 인 경우도 렌더링 로직은 동일하게 처리하기 위해서 도입
	LineBatch *getLineBatch3D(float thickness, bool depthEnabled);
	LineBatch *getLineBatch2D(float thickness);

	// 해괴한 편법일지도 모르겠지만
	// 선의 두께에 해당하는것은 음수가 될리가 없잖아? 그래서 편법으로
	// key > 0 => depth enabled
	// key < 0 => depth disabled
	std::map<float, LineBatch*> batch3DMap_;
	std::map<float, LineBatch*> batch2DMap_;

	LineBatch *defaultBatch3DWithDepth_;
	LineBatch *defaultBatch3DWithoutDepth_;
	LineBatch *defaultBatch2D_;

	irr::IrrlichtDevice *device_;
};

extern irr::gui::IGUIFont *g_normalFont12;
extern irr::gui::IGUIFont *g_normalFont14;

