// Ŭnicode please 
#include "stdafx.h"
#include "debug_drawer.h"
#include "wire_sphere_factory.h"
#include "irr/line_batch_scene_node.h"

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

DebugDrawer::DebugDrawer(irr::IrrlichtDevice *dev)
	: device_(dev), batchSceneNode_(nullptr)
{
	if(dev != nullptr) {
		ISceneManager* smgr = dev->getSceneManager();
		batchSceneNode_ = new LineBatchSceneNode(smgr->getRootSceneNode(), smgr, 0);
	}
}

DebugDrawer::~DebugDrawer()
{
	shutDown();
}

void DebugDrawer::shutDown()
{
	if(batchSceneNode_) {
		batchSceneNode_->remove();
		batchSceneNode_->drop();
		batchSceneNode_ = nullptr;
	}
	auto it = batchSceneNodeMap_.begin();
	auto endit = batchSceneNodeMap_.end();
	for( ; it != endit ; ++it) {
		it->second->remove();
		it->second->drop();
	}
}

LineBatchSceneNode *DebugDrawer::getBatchSceneNode(float thickness)
{
	if(thickness == 1.0f || abs(thickness - 1.0f) < FLT_EPSILON) {
		return batchSceneNode_;
	}
	auto found = batchSceneNodeMap_.find(thickness);
	if(found != batchSceneNodeMap_.end()) {
		return found->second;
	}
	ISceneManager* smgr = device_->getSceneManager();
	auto sceneNode = new LineBatchSceneNode(smgr->getRootSceneNode(), smgr, 0);
	sceneNode->setThickness(thickness);
	batchSceneNodeMap_[thickness] = sceneNode;
	return sceneNode;
}

void DebugDrawer::drawAll(const DebugDrawManager &mgr)
{
	if(batchSceneNode_) {
		batchSceneNode_->clear();
	}
	auto it = batchSceneNodeMap_.begin();
	const auto endit = batchSceneNodeMap_.end();
	for( ; it != endit ; ++it) {
		it->second->clear();
	}
	
	DrawFunctor<DrawAttributeElemList>::draw(this, mgr);
	
	if(batchSceneNode_) {
		batchSceneNode_->render();
	}
	it = batchSceneNodeMap_.begin();
	for( ; it != endit ; ++it) {
		it->second->render();
	}
}


void DebugDrawer::drawList(const DrawAttributeList_Line2D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.scaleList[i];
		auto sceneNode = getBatchSceneNode(scale);
		sceneNode->addLine(cmd.p1List[i], cmd.p2List[i], cmd.colorList[i]);
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

		auto sceneNode = getBatchSceneNode(1.0f);
		sceneNode->addLine(top, bottom, color);
		sceneNode->addLine(left, right, color);
	}
}
void DebugDrawer::drawList(const DrawAttributeList_String2D &cmd)
{
	IGUIFont *font = getDebugFont();

	auto driver = device_->getVideoDriver();
	const irr::core::dimension2du& screenSize = driver->getScreenSize();
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

		auto sceneNode = getBatchSceneNode(1.0f);
		sceneNode->addIndexedVertices2D(vertexList, baseIndexList);
	}
}

void DebugDrawer::drawList(const DrawAttributeList_Line3D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.scaleList[i];
		auto sceneNode = getBatchSceneNode(scale);
		sceneNode->addLine(cmd.p1List[i], cmd.p2List[i], cmd.colorList[i]);
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

		std::copy(baseVertexList.begin(), baseVertexList.end(), vertexList.begin());
		for(auto &vert : vertexList) {
			vert.Color = color;
			vert.Pos *= scale;
			vert.Pos += pos;
		}

		auto sceneNode = getBatchSceneNode(1.0f);
		sceneNode->addIndexedVertices(vertexList, baseIndexList);
	}
}
void DebugDrawer::drawList(const DrawAttributeList_String3D &cmd)
{
	ISceneManager *smgr = device_->getSceneManager();
	ICameraSceneNode *cam = smgr->getActiveCamera();
	ISceneCollisionManager *coll = device_->getSceneManager()->getSceneCollisionManager();
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

		vector3df zero(0, 0, 0);
		vector3df x(size, 0, 0);
		vector3df y(0, size, 0);
		vector3df z(0, 0, size);

		xf.transformVect(zero);
		xf.transformVect(x);
		xf.transformVect(y);
		xf.transformVect(z);

		auto sceneNode = getBatchSceneNode(1.0f);
		sceneNode->addLine(zero, x, red);
		sceneNode->addLine(zero, y, green);
		sceneNode->addLine(zero, z, blue);
	}
}

irr::gui::IGUIFont *DebugDrawer::getDebugFont()
{
	IGUIFont *font = g_normalFont14;
	if(font == nullptr) {
		font = device_->getGUIEnvironment()->getBuiltInFont();
	}
	return font;
}

irr::gui::IGUIFont *g_normalFont12 = nullptr;
irr::gui::IGUIFont *g_normalFont14 = nullptr;