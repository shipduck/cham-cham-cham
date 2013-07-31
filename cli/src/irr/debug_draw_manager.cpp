// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_manager.h"
#include "wire_sphere_factory.h"
#include "irr/line_batch_scene_node.h"

using namespace Loki;
using namespace irr;
using namespace video;
using namespace scene;
using namespace core;
using namespace gui;

//singleton
DebugDrawManager debugDrawMgrLocal;
DebugDrawManager *g_debugDrawMgr = &debugDrawMgrLocal;

irr::gui::IGUIFont *g_normalFont12 = nullptr;
irr::gui::IGUIFont *g_normalFont14 = nullptr;



void DebugDrawManager::startUp(irr::IrrlichtDevice *dev)
{
	this->device_ = dev;

	if(dev != nullptr) {
		ISceneManager* smgr = dev->getSceneManager();
		batchSceneNode_ = new LineBatchSceneNode(smgr->getRootSceneNode(), smgr, 0);
	}
}
void DebugDrawManager::shutDown()
{
	clear();
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

LineBatchSceneNode *DebugDrawManager::getBatchSceneNode(float thickness)
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

template<typename TList> struct DebugDrawListFunctor;
template<>
struct DebugDrawListFunctor<NullType> {
	static int size(const DebugDrawManager &mgr) { return 0; }
	static void draw(DebugDrawManager &mgr) { return; }
	static void clear(DebugDrawManager &mgr) { return; }
	static void update(int ms, DebugDrawManager &mgr) { return; }
};
template<typename T, typename U>
struct DebugDrawListFunctor< Typelist<T, U> > {
	static int size(const DebugDrawManager &mgr)
	{
		int sum = 0;
		sum += Field<T>(mgr).immediateDrawList.size();
		sum += Field<T>(mgr).durationDrawList.size();
		sum += DebugDrawListFunctor<U>::size(mgr);
		return sum;
	}
	static void draw(DebugDrawManager &mgr)
	{
		const auto &immediateList = Field<T>(mgr).immediateDrawList;
		const auto &durationList = Field<T>(mgr).durationDrawList;
		if(immediateList.size() > 0) {
			mgr.drawList(immediateList);
		}
		if(durationList.size() > 0) {
			mgr.drawList(durationList);
		}
		DebugDrawListFunctor<U>::draw(mgr);
	}
	static void clear(DebugDrawManager &mgr)
	{
		Field<T>(mgr).durationList.clear();
		Field<T>(mgr).durationDrawList.clear();
		Field<T>(mgr).immediateDrawList.clear();
		DebugDrawListFunctor<U>::clear(mgr);
	}
	static void update(int ms, DebugDrawManager &mgr)
	{
		Field<T>(mgr).immediateDrawList.clear();
		
		auto &durationList = Field<T>(mgr).durationList;
		auto &durationDrawList = Field<T>(mgr).durationDrawList;
		SR_ASSERT(durationList.size() == durationDrawList.size());

		//경과 시간 처리
		for(size_t i = 0 ; i < durationList.size() ; ++i) {
			durationList[i] -= ms;
		}
		while(durationList.empty() == false) {
			int duration = durationList.back();
			if(duration > 0) {
				break;
			}
			durationList.pop_back();
		}

		typedef std::tuple<int, int> DurationIndexTuple;
		std::vector<DurationIndexTuple> durationIndexList(durationList.size());
		for(size_t i = 0 ; i < durationList.size() ; ++i) {
			DurationIndexTuple &dataTuple = durationIndexList[i];
			std::get<0>(dataTuple) = durationList[i];
			std::get<1>(dataTuple) = i;
		}
		//durationDrawList-durationList 시간 내림차순으로 정렬
		std::sort(durationIndexList.begin(),
			durationIndexList.end(),
			[](const DurationIndexTuple &a, const DurationIndexTuple &b) {
				return std::get<0>(a) > std::get<0>(b);
		});
		std::sort(durationList.begin(), 
			durationList.end(),
			[](int a, int b) { return a > b; });

		//index 순서대로 복사, 경과시간 고려해서 지나친항목은 무시
		typedef std::remove_reference<decltype(durationDrawList)>::type DurationDrawList;
		DurationDrawList nextDurationDrawList;
		for(const auto &dataTuple : durationIndexList) {
			int idx = std::get<1>(dataTuple);
			int duration = std::get<0>(dataTuple);
			if(duration > 0) {
				durationDrawList.copy_elem(&nextDurationDrawList, idx);
			}
		}
		durationDrawList = std::move(nextDurationDrawList);

		SR_ASSERT(durationList.size() == durationDrawList.size());
		DebugDrawListFunctor<U>::update(ms, mgr);
	}
};

size_t DebugDrawManager::size() const
{
	int sum = DebugDrawListFunctor<DrawAttributeElemList>::size(*this);	
	return sum;
}


void DebugDrawManager::drawAll()
{
	if(batchSceneNode_) {
		batchSceneNode_->clear();
	}
	auto it = batchSceneNodeMap_.begin();
	const auto endit = batchSceneNodeMap_.end();
	for( ; it != endit ; ++it) {
		it->second->clear();
	}
	
	DebugDrawListFunctor<DrawAttributeElemList>::draw(*this);
	
	if(batchSceneNode_) {
		batchSceneNode_->render();
	}
	it = batchSceneNodeMap_.begin();
	for( ; it != endit ; ++it) {
		it->second->render();
	}
}

void DebugDrawManager::clear() 
{
	DebugDrawListFunctor<DrawAttributeElemList>::clear(*this);
}

void DebugDrawManager::updateAll(int ms)
{
	DebugDrawListFunctor<DrawAttributeElemList>::update(ms, *this);
}

void DebugDrawManager::addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Line3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.p1List.push_back(p1);
	drawList.p2List.push_back(p2);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(lineWidth);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCross(const irr::core::vector3df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Cross3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(size);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addSphere(const irr::core::vector3df &pos, 
		float radius,
		const irr::video::SColor &color,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Sphere3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(radius);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Axis3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.xfList.push_back(xf);
	drawList.scaleList.push_back(size);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addString(const irr::core::vector3df &pos, 
		const DrawAttributeListMixin_String::string_type &msg,
		const irr::video::SColor &color,
		float scale,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_String3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(scale);
	drawList.msgList.push_back(msg);
	drawList.depthEnableList.push_back(depthEnable);

	//create scene node
	ISceneManager* smgr = device_->getSceneManager();
	ITextSceneNode *node = smgr->addTextSceneNode(g_normalFont14, msg.data(), color);
	if(node) {
		SMaterial material;
		material.Wireframe = true;
		material.Lighting = false;
		material.BackfaceCulling = false;
		material.FrontfaceCulling = false;
		node->getMaterial(0) = material;
	}
	node->setScale(vector3df(scale, scale, scale));
	node->setPosition(pos);
	node->grab();
	drawList.nodeList.push_back(node);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration)
{
	typedef DrawAttributeList_Line2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.p1List.push_back(p1);
	drawList.p2List.push_back(p2);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(lineWidth);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCross(const irr::core::vector2di &pos, 
		const irr::video::SColor &color,
		float size,
		int duration)
{
	typedef DrawAttributeList_Cross2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(size);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addString(const irr::core::vector2di &pos, 
								   const DrawAttributeListMixin_String::string_type &msg,
								   const irr::video::SColor &color,
								   float scale,
								   int duration)
{
	typedef DrawAttributeList_String2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(scale);
	drawList.msgList.push_back(msg);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration)
{
	typedef DrawAttributeList_Circle2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(radius);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::drawList(const DrawAttributeList_Line2D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.scaleList[i];
		auto sceneNode = getBatchSceneNode(scale);
		sceneNode->addLine(cmd.p1List[i], cmd.p2List[i], cmd.colorList[i]);
	}
}

void DebugDrawManager::drawList(const DrawAttributeList_Cross2D &cmd)
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
void DebugDrawManager::drawList(const DrawAttributeList_String2D &cmd)
{
	if(!g_normalFont14) {
		return;
	}

	auto driver = device_->getVideoDriver();
	const irr::core::dimension2du& screenSize = driver->getScreenSize();
	int w = screenSize.Width;
	int h = screenSize.Height;

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		const auto &pos = cmd.posList[i];
		const auto &color = cmd.colorList[i];
		auto msg = cmd.msgList[i];
		int x = pos.X;
		int y = pos.Y;
		g_normalFont14->draw(msg.data(), rect<s32>(x, y, w, h), color);
	}
}

void DebugDrawManager::drawList(const DrawAttributeList_Circle2D &cmd)
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

void DebugDrawManager::drawList(const DrawAttributeList_Line3D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.scaleList[i];
		auto sceneNode = getBatchSceneNode(scale);
		sceneNode->addLine(cmd.p1List[i], cmd.p2List[i], cmd.colorList[i]);
	}
}

void DebugDrawManager::drawList(const DrawAttributeList_Cross3D &cmd)
{
	//항상 동일한 크기로 보이게 적절히 렌더링하기
}

void DebugDrawManager::drawList(const DrawAttributeList_Sphere3D &cmd)
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
void DebugDrawManager::drawList(const DrawAttributeList_String3D &cmd)
{
	//use scene node based
}

void DebugDrawManager::drawList(const DrawAttributeList_Axis3D &cmd)
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
