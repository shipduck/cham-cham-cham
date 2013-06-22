// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_manager.h"

using namespace Loki;
using namespace irr;
using namespace video;
using namespace scene;
using namespace core;
using namespace gui;

//singleton
DebugDrawManager debugDrawMgrLocal;
DebugDrawManager *gDebugDrawMgr = &debugDrawMgrLocal;

irr::gui::IGUIFont *gNormalFont12 = nullptr;
irr::gui::IGUIFont *gNormalFont14 = nullptr;


DebugDrawListMixin_Node::~DebugDrawListMixin_Node()
{
	clear();
}
void DebugDrawListMixin_Node::pop_back() 
{
	auto node = NodeList.back();
	if(node) {
		node->remove();
		node->drop();
	}
	NodeList.pop_back();
}
void DebugDrawListMixin_Node::clear() 
{
	auto it = NodeList.begin();
	auto endit = NodeList.end();
	for( ; it != endit ; ++it) {
		auto node = *it;
		if(node) {
			node->remove();
			node->drop();
		}
	}
	NodeList.clear(); 
}

void DebugDrawManager::setUp(irr::IrrlichtDevice *dev)
{
	this->Device = dev;
}
void DebugDrawManager::shutDown()
{
	clear();
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
		sum += Field<T>(mgr).ImmediateDrawList.size();
		sum += Field<T>(mgr).DurationDrawList.size();
		sum += DebugDrawListFunctor<U>::size(mgr);
		return sum;
	}
	static void draw(DebugDrawManager &mgr)
	{
		const auto &immediateList = Field<T>(mgr).ImmediateDrawList;
		const auto &durationList = Field<T>(mgr).DurationDrawList;
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
		Field<T>(mgr).DurationList.clear();
		Field<T>(mgr).DurationDrawList.clear();
		Field<T>(mgr).ImmediateDrawList.clear();
		DebugDrawListFunctor<U>::clear(mgr);
	}
	static void update(int ms, DebugDrawManager &mgr)
	{
		Field<T>(mgr).ImmediateDrawList.clear();
		
		auto &durationList = Field<T>(mgr).DurationList;
		auto &durationDrawList = Field<T>(mgr).DurationDrawList;

		//TODO DurationDrawList-DurationList 시간 내림차순으로 정렬
		SR_ASSERT(durationList.size() == 0 && "duration based is not implemented yet");

		//시간지난거 삭제
		for(size_t i = 0 ; i < durationList.size() ; ++i) {
			durationList[i] -= ms;
		}
		while(durationList.empty() == false) {
			int duration = durationList.back();
			if(duration > 0) {
				break;
			}

			durationList.pop_back();
			durationDrawList.pop_back();
		}

		DebugDrawListFunctor<U>::update(ms, mgr);
	}
};

int DebugDrawManager::size() const
{
	int sum = DebugDrawListFunctor<DebugDrawCmdTypeList>::size(*this);	
	return sum;
}


void DebugDrawManager::drawAll()
{
	DebugDrawListFunctor<DebugDrawCmdTypeList>::draw(*this);
}

void DebugDrawManager::clear() 
{
	DebugDrawListFunctor<DebugDrawCmdTypeList>::clear(*this);
}

void DebugDrawManager::updateAll(int ms)
{
	DebugDrawListFunctor<DebugDrawCmdTypeList>::update(ms, *this);
}

void DebugDrawManager::addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_Line3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.P1List.push_back(p1);
	drawList.P2List.push_back(p2);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(lineWidth);
	drawList.DepthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCross(const irr::core::vector3df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_Cross3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(size);
	drawList.DepthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addSphere(const irr::core::vector3df &pos, 
		float radius,
		const irr::video::SColor &color,
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_Sphere3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(radius);
	drawList.DepthEnableList.push_back(depthEnable);

	//create scene node
	ISceneManager* smgr = Device->getSceneManager();
	IMeshSceneNode *node = smgr->addSphereSceneNode(radius);
	if(node) {
		SMaterial material;
		material.Wireframe = true;
		material.Lighting = false;
		material.BackfaceCulling = false;
		material.FrontfaceCulling = false;
		node->getMaterial(0) = material;
		smgr->getMeshManipulator()->setVertexColors(node->getMesh(), color);
	}
	node->setPosition(pos);
	node->grab();
	drawList.NodeList.push_back(node);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_Axis3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.XfList.push_back(xf);
	drawList.ScaleList.push_back(size);
	drawList.DepthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addString(const irr::core::vector3df &pos, 
		const DebugDrawListMixin_String::string_type &msg,
		const irr::video::SColor &color,
		float scale,
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_String3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(scale);
	drawList.MsgList.push_back(msg);
	drawList.DepthEnableList.push_back(depthEnable);

	//create scene node
	ISceneManager* smgr = Device->getSceneManager();
	ITextSceneNode *node = smgr->addTextSceneNode(gNormalFont14, msg.data(), color);
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
	drawList.NodeList.push_back(node);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration)
{
	typedef DebugDrawList_Line2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.P1List.push_back(p1);
	drawList.P2List.push_back(p2);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(lineWidth);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCross(const irr::core::vector2di &pos, 
		const irr::video::SColor &color,
		float size,
		int duration)
{
	typedef DebugDrawList_Cross2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(size);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addString(const irr::core::vector2di &pos, 
								   const DebugDrawListMixin_String::string_type &msg,
								   const irr::video::SColor &color,
								   float scale,
								   int duration)
{
	typedef DebugDrawList_String2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(scale);
	drawList.MsgList.push_back(msg);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration)
{
	typedef DebugDrawList_Circle2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(radius);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::drawList(const DebugDrawList_Line2D &cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, core::matrix4());

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		video::SMaterial m; 
		m.Lighting = false;
		m.Thickness = cmd.ScaleList[i];
		driver->setMaterial(m);
		driver->draw2DLine(cmd.P1List[i], cmd.P2List[i], cmd.ColorList[i]);
	}
}

void DebugDrawManager::drawList(const DebugDrawList_Cross2D &cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	video::SMaterial m; 
	m.Lighting = false;
	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());             

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		auto pos = cmd.PosList[i];
		auto size = static_cast<int>(cmd.ScaleList[i]);
		auto color = cmd.ColorList[i];

		vector2di top = pos + vector2di(0, size);
		vector2di bottom = pos - vector2di(0, size);
		vector2di right = pos + vector2di(size, 0);
		vector2di left = pos - vector2di(size, 0);

		driver->draw2DLine(top, bottom, color);
		driver->draw2DLine(right, left, color);
	}
}
void DebugDrawManager::drawList(const DebugDrawList_String2D &cmd)
{
	if(!gNormalFont14) {
		return;
	}

	auto driver = Device->getVideoDriver();
	const irr::core::dimension2du& screenSize = driver->getScreenSize();
	int w = screenSize.Width;
	int h = screenSize.Height;

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		auto pos = cmd.PosList[i];
		auto color = cmd.ColorList[i];
		auto msg = cmd.MsgList[i];
		int x = pos.X;
		int y = pos.Y;
		gNormalFont14->draw(msg.data(), rect<s32>(x, y, w, h), color);
	}
}

void DebugDrawManager::drawList(const DebugDrawList_Circle2D &cmd)
{
	SR_ASSERT(false && "NotImplemented");
}

void DebugDrawManager::drawList(const DebugDrawList_Line3D &cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, core::matrix4());

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		video::SMaterial m; 
		m.Lighting = false;
		m.Thickness = cmd.ScaleList[i];
		driver->setMaterial(m);
		driver->draw3DLine(cmd.P1List[i], cmd.P2List[i], cmd.ColorList[i]);
	}
}

void DebugDrawManager::drawList(const DebugDrawList_Cross3D &cmd)
{
	//항상 동일한 크기로 보이게 적절히 렌더링하기
}

void DebugDrawManager::drawList(const DebugDrawList_Sphere3D &cmd)
{
	//use scene node based
}
void DebugDrawManager::drawList(const DebugDrawList_String3D &cmd)
{
	//use scene node based
}

void DebugDrawManager::drawList(const DebugDrawList_Axis3D &cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	video::SMaterial m; 
	m.Lighting = false;
	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());

	SColor red(255, 255, 0, 0);
	SColor green(255, 0, 255, 0);
	SColor blue(255, 0, 0, 255);

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		auto size = cmd.ScaleList[i];
		auto xf = cmd.XfList[i];

		vector3df zero(0, 0, 0);
		vector3df x(size, 0, 0);
		vector3df y(0, size, 0);
		vector3df z(0, 0, size);

		xf.transformVect(zero);
		xf.transformVect(x);
		xf.transformVect(y);
		xf.transformVect(z);

		driver->draw3DLine(zero, x, red);
		driver->draw3DLine(zero, y, green);
		driver->draw3DLine(zero, z, blue);
	}
}