// Ŭnicode please 
#include "stdafx.h"
#include "debug_drawer.h"
#include "wire_sphere_factory.h"
#include "line_batch.h"
#include "base/lib.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace Loki;
using namespace gui;

template<typename TList> struct DrawFunctor;
template<>
struct DrawFunctor<NullType> {
	static void draw(DebugDrawer *drawer, const DebugDrawManager &mgr) { return; }
};
template<typename T, typename U>
struct DrawFunctor< Typelist<T, U> > {
	static void draw(DebugDrawer *drawer, const DebugDrawManager &mgr)
	{
		const auto &immediateList = Field<T>(mgr).immediateDrawList;
		const auto &durationList = Field<T>(mgr).durationDrawList;
		if(immediateList.size() > 0) {
			drawer->drawList(immediateList);
		}
		if(durationList.size() > 0) {
			drawer->drawList(durationList);
		}
		DrawFunctor<U>::draw(drawer, mgr);
	}
};

DebugDrawer::DebugDrawer()
	: defaultBatch3DWithoutDepth_(nullptr),
	defaultBatch3DWithDepth_(nullptr),
	defaultBatch2D_(nullptr)
{
	batch3DMap_[1.0f] = new LineBatch(1.0f);
	batch3DMap_[-1.0f] = new LineBatch(1.0f);
	batch2DMap_[1.0f] = new LineBatch(1.0f);
	
	defaultBatch3DWithDepth_ = batch3DMap_[1.0f];
	defaultBatch3DWithoutDepth_ = batch3DMap_[-1.0f];
	defaultBatch2D_ = batch2DMap_[1.0f];	
}
DebugDrawer::~DebugDrawer()
{
	for(auto x : batch3DMap_) {
		delete(x.second);
	}
	for(auto x : batch2DMap_) {
		delete(x.second);
	}
}

LineBatch *DebugDrawer::getLineBatch3D(float thickness, bool depthEnable)
{
	if(thickness == 1.0f || abs(thickness - 1.0f) < FLT_EPSILON) {
		if(depthEnable) {
			return defaultBatch3DWithDepth_;
		} else {
			return defaultBatch3DWithoutDepth_;
		}
	}

	float key = thickness;
	if(depthEnable == false) {
		key = -key;
	}

	auto found = batch3DMap_.find(key);
	if(found != batch3DMap_.end()) {
		return found->second;
	}
	LineBatch *batch = new LineBatch(thickness);
	batch3DMap_[key] = batch;
	return batch;
}

LineBatch *DebugDrawer::getLineBatch2D(float thickness)
{
	if(thickness == 1.0f || abs(thickness - 1.0f) < FLT_EPSILON) {
		return defaultBatch2D_;
	}

	float key = thickness;

	auto found = batch2DMap_.find(key);
	if(found != batch2DMap_.end()) {
		return found->second;
	}
	LineBatch *batch = new LineBatch(thickness);
	batch2DMap_[key] = batch;
	return batch;
}

void DebugDrawer::drawAll(const DebugDrawManager &mgr)
{
	for(auto it : batch3DMap_) {
		it.second->clear();
	}
	for(auto it : batch2DMap_) {
		it.second->clear();
	}
	
	DrawFunctor<DrawAttributeElemList>::draw(this, mgr);

	IVideoDriver *driver = Lib::driver;
	for(auto it : batch3DMap_) {
		if(it.first > 0) {
			it.second->draw(driver, true);
		} else {
			it.second->draw(driver, false);
		}
	}
	for(auto it : batch2DMap_) {
		it.second->draw2D(driver);
	}
}


void DebugDrawer::drawList(const DrawAttributeList_Line2D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.scaleList[i];
		auto batch = getLineBatch2D(scale);
		batch->add(cmd.p1List[i], cmd.p2List[i], cmd.colorList[i]);
	}
}

void DebugDrawer::drawList(const DrawAttributeList_Cross2D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		const auto &pos = cmd.posList[i];
		auto size = static_cast<int>(cmd.scaleList[i]);
		const auto &color = cmd.colorList[i];

		vector2di top = pos + vector2di(0, size);
		vector2di bottom = pos - vector2di(0, size);
		vector2di right = pos + vector2di(size, 0);
		vector2di left = pos - vector2di(size, 0);

		auto batch = getLineBatch2D(1.0f);
		batch->add(top, bottom, color);
		batch->add(left, right, color);
	}
}
void DebugDrawer::drawList(const DrawAttributeList_String2D &cmd)
{
	IGUIFont *font = getDebugFont();
	const irr::core::dimension2du& screenSize = Lib::driver->getScreenSize();
	int w = screenSize.Width;
	int h = screenSize.Height;

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		const auto &pos = cmd.posList[i];
		const auto &color = cmd.colorList[i];
		const auto &text = cmd.textList[i];
		int x = pos.X;
		int y = pos.Y;
		font->draw(text.data(), rect<s32>(x, y, w, h), color);
	}
}

void DebugDrawer::drawList(const DrawAttributeList_Circle2D &cmd)
{
	const int numSegment = 16;
	static std::array<S3DVertex, numSegment> baseVertexList;
	static std::array<unsigned short, numSegment * 2> baseIndexList;
	static bool init = false;
	if(init == false) {
		init = true;

		SColor white(255, 255, 255, 255);
		const float segRad = 2 * PI / numSegment;
		for(int i = 0 ; i < numSegment ; ++i) {
			auto &vert = baseVertexList[i];
			float x = cos(segRad * i);
			float y = sin(segRad * i);
			vert.Pos.X = x;
			vert.Pos.Y = y;
			vert.Pos.Z = 0;
		}

		for(int i = 0 ; i < numSegment ; i++) {
			int a = i;
			int b = (i + 1) % numSegment;
			baseIndexList[i*2] = a;
			baseIndexList[i*2 + 1] = b;
		}
	}

	std::array<S3DVertex, numSegment> vertexList;
	for(size_t i = 0 ; i < cmd.size() ; ++i) {
		const auto &pos = cmd.posList[i];
		const auto &color = cmd.colorList[i];
		float radius = cmd.scaleList[i];

		std::copy(baseVertexList.begin(), baseVertexList.end(), vertexList.begin());
		for(auto &vert : vertexList) {
			vert.Color = color;
			vert.Pos *= radius;
			vert.Pos += vector3df(static_cast<float>(pos.X), static_cast<float>(pos.Y), 0.0f);
		}

		auto batch = getLineBatch2D(1.0f);
		batch->addIndexedVertices(vertexList, baseIndexList);
	}
}

void DebugDrawer::drawList(const DrawAttributeList_Line3D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.scaleList[i];
		bool depthEnable = cmd.depthEnableList[i];
		auto batch = getLineBatch3D(scale, depthEnable);
		batch->add(cmd.p1List[i], cmd.p2List[i], cmd.colorList[i]);
	}
}

void DebugDrawer::drawList(const DrawAttributeList_Cross3D &cmd)
{
	//항상 동일한 크기로 보이게 적절히 렌더링하기
}

void DebugDrawer::drawList(const DrawAttributeList_Sphere3D &cmd)
{
	typedef WireSphereFactory<8, 8> SphereFactoryType;
	static SphereFactoryType::vertex_list_type baseVertexList;
	static SphereFactoryType::index_list_type baseIndexList;
	static bool init = false;
	if(init == false) {
		init = true;
		SphereFactoryType::createSimpleMesh(baseVertexList, baseIndexList);
	}
	
	SphereFactoryType::vertex_list_type vertexList;
	for(size_t i = 0 ; i < cmd.size() ; i++) {
		const auto &color = cmd.colorList[i];
		const auto &pos = cmd.posList[i];
		auto scale = cmd.scaleList[i];
		bool depthEnable = cmd.depthEnableList[i];

		std::copy(baseVertexList.begin(), baseVertexList.end(), vertexList.begin());
		for(auto &vert : vertexList) {
			vert.Color = color;
			vert.Pos *= scale;
			vert.Pos += pos;
		}

		auto batch = getLineBatch3D(1.0f, depthEnable);
		batch->addIndexedVertices(vertexList, baseIndexList);
	}
}
void DebugDrawer::drawList(const DrawAttributeList_String3D &cmd)
{
	ISceneManager *smgr = Lib::device->getSceneManager();
	ICameraSceneNode *cam = smgr->getActiveCamera();
	ISceneCollisionManager *coll = smgr->getSceneCollisionManager();
	IGUIFont *font = getDebugFont();

	for(size_t i = 0 ; i < cmd.size() ; ++i) {
		const auto &color = cmd.colorList[i];
		const auto &pos = cmd.posList[i];
		const float scale = cmd.scaleList[i];
		const auto &text = cmd.textList[i];		

		//text scene node의 렌더링을 적절히 재탕
		//scene node를 쓰면 메모리 관리를 신경써야되는데
		//low-level에서 렌더링하게 하면 좀 편하다
		core::position2d<s32> p = coll->getScreenCoordinatesFrom3DPosition(pos, cam);
		core::rect<s32> r(p, core::dimension2d<s32>(1,1));
		font->draw(text.c_str(), r, color, true, true);
	}
}

void DebugDrawer::drawList(const DrawAttributeList_Axis3D &cmd)
{
	SColor red(255, 255, 0, 0);
	SColor green(255, 0, 255, 0);
	SColor blue(255, 0, 0, 255);

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		auto size = cmd.scaleList[i];
		auto xf = cmd.xfList[i];
		bool depthEnable = cmd.depthEnableList[i];

		vector3df zero(0, 0, 0);
		vector3df x(size, 0, 0);
		vector3df y(0, size, 0);
		vector3df z(0, 0, size);

		xf.transformVect(zero);
		xf.transformVect(x);
		xf.transformVect(y);
		xf.transformVect(z);

		auto batch = getLineBatch3D(1.0f, depthEnable);
		batch->add(zero, x, red);
		batch->add(zero, y, green);
		batch->add(zero, z, blue);
	}
}

irr::gui::IGUIFont *normalFont12 = nullptr;
irr::gui::IGUIFont *normalFont14 = nullptr;
irr::gui::IGUIFont *debugFont = nullptr;

irr::gui::IGUIFont *getFont(const char *fontName)
{
	SR_ASSERT(Lib::guienv != nullptr);
	IGUIFont *font = Lib::guienv->getFont(fontName);
	if(font == nullptr) {
		Lib::warning("Cannot find Font : %s, use Built-in font", fontName);
		font = Lib::guienv->getBuiltInFont();
	}
	return font;
}


irr::gui::IGUIFont *getDebugFont()
{
	if(debugFont == nullptr) {
		const char *fontName = "res/font/console-14.xml";
		debugFont = getFont(fontName);
	}
	return debugFont;
}

irr::gui::IGUIFont *getNormalFont12()
{
	if(normalFont12 == nullptr) {
		const char *fontName = "res/font/font_12.xml";
		normalFont12 = getFont(fontName);
	}
	return normalFont12;
}

irr::gui::IGUIFont *getNormalFont14()
{
	if(normalFont14 == nullptr) {
		const char *fontName = "res/font/font_14.xml";
		normalFont14 = getFont(fontName);
	}
	return normalFont14;
}