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

void DebugDrawListMixin_Color::clear() 
{
	ColorList.clear();
}
void DebugDrawListMixin_Color::pop_back() 
{
	ColorList.pop_back();
}
int DebugDrawListMixin_Color::size() const
{
	return ColorList.size();
}

void DebugDrawListMixin_Node::clear() 
{
	NodeList.clear();
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
int DebugDrawListMixin_Node::size() const
{
	return NodeList.size();
}

void DebugDrawListMixin_3D::clear() 
{
	DepthEnable.clear();
}
void DebugDrawListMixin_3D::pop_back()
{
	DepthEnable.pop_back();
}
int DebugDrawListMixin_3D::size() const
{
	return DepthEnable.size();
}

void DebugDrawListMixin_LineWidth::clear()
{
	LineWidthList.clear();
}
void DebugDrawListMixin_LineWidth::pop_back()
{
	LineWidthList.pop_back();
}
int DebugDrawListMixin_LineWidth::size() const
{
	return LineWidthList.size();
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
		mgr.drawList(immediateList);
		mgr.drawList(durationList);
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
	DebugDrawList_Line3D *drawList = nullptr;
	SR_ASSERT(duration >= 0);
	if(duration == 0) {
		drawList = &Field<DebugDrawList_Line3D>(*this).ImmediateDrawList;
	} else {
		drawList = &Field<DebugDrawList_Line3D>(*this).DurationDrawList;
	}
	
	drawList->P1List.push_back(p1);
	drawList->P2List.push_back(p2);
	drawList->ColorList.push_back(color);
	drawList->LineWidthList.push_back(lineWidth);
	drawList->DepthEnable.push_back(depthEnable);

	if(duration > 0) {
		Field<DebugDrawList_Line3D>(*this).DurationList.push_back(duration);
	}
}

/*
void DebugDrawManager::addCross(const irr::core::vector3df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration,
		bool depthEnable)
{
	typedef DebugDraw_Cross3D CmdType;
	CmdType cmd;
	cmd.Pos = pos;
	cmd.Color = color;
	cmd.Size = size;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;
	Field<CmdType>(*this).getList().push_back(cmd);
}

void DebugDrawManager::addSphere(const irr::core::vector3df &pos, 
		float radius,
		const irr::video::SColor &color,
		int duration,
		bool depthEnable)
{
	typedef DebugDraw_Sphere3D CmdType;
	CmdType cmd;
	cmd.Pos = pos;
	cmd.Radius = radius;
	cmd.Color = color;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;

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
	cmd.Node = node;

	Field<CmdType>(*this).getList().push_back(cmd);
}

void DebugDrawManager::addAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	typedef DebugDraw_Axis3D CmdType;
	CmdType cmd;
	cmd.Xf = xf;
	cmd.Size = size;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;
	Field<CmdType>(*this).getList().push_back(cmd);
}

void DebugDrawManager::addString(const irr::core::vector3df &pos, 
		const DebugDraw::string_type &msg,
		const irr::video::SColor &color,
		float scale,
		int duration,
		bool depthEnable)
{
	typedef DebugDraw_String3D CmdType;
	CmdType cmd;
	cmd.Pos = pos;
	cmd.Msg = msg;
	cmd.Color = color;
	cmd.Scale = scale;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;

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
	cmd.Node = node;

	Field<CmdType>(*this).getList().push_back(cmd);
}
*/
void DebugDrawManager::addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration)
{
	DebugDrawList_Line2D *drawList = nullptr;
	SR_ASSERT(duration >= 0);
	if(duration == 0) {
		drawList = &Field<DebugDrawList_Line2D>(*this).ImmediateDrawList;
	} else {
		drawList = &Field<DebugDrawList_Line2D>(*this).DurationDrawList;
	}
	
	drawList->P1List.push_back(p1);
	drawList->P2List.push_back(p2);
	drawList->ColorList.push_back(color);
	drawList->LineWidthList.push_back(lineWidth);

	if(duration > 0) {
		Field<DebugDrawList_Line2D>(*this).DurationList.push_back(duration);
	}
}
/*
void DebugDrawManager::addCross(const irr::core::vector2di &pos, 
		const irr::video::SColor &color,
		float size,
		int duration)
{
	typedef DebugDraw_Cross2D CmdType;
	CmdType cmd;
	cmd.Pos = pos;
	cmd.Color = color;
	cmd.Size = size;
	cmd.Duration = duration;
	Field<CmdType>(*this).getList().push_back(cmd);
}

void DebugDrawManager::addString(const irr::core::vector2di &pos, 
								   const DebugDraw::string_type &msg,
								   const irr::video::SColor &color,
								   float scale,
								   int duration)
{
	typedef DebugDraw_String2D CmdType;
	CmdType cmd;
	cmd.Pos = pos;
	cmd.Msg = msg;
	cmd.Color = color;
	cmd.Scale = scale;
	cmd.Duration = duration;
	Field<CmdType>(*this).getList().push_back(cmd);
}

void DebugDrawManager::addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration)
{
	typedef DebugDraw_Circle2D CmdType;
	CmdType cmd;
	cmd.Pos = pos;
	cmd.Radius = radius;
	cmd.Color = color;
	cmd.Duration = duration;
	Field<CmdType>(*this).getList().push_back(cmd);
}
*/
void DebugDrawManager::drawList(const DebugDrawList_Line2D &cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, core::matrix4());

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		video::SMaterial m; 
		m.Lighting = false;
		m.Thickness = cmd.LineWidthList[i];
		driver->setMaterial(m);
		driver->draw2DLine(cmd.P1List[i], cmd.P2List[i], cmd.ColorList[i]);
	}
}
/*
void DebugDrawManager::drawElem(const DebugDraw_Cross2D *cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	video::SMaterial m; 
	m.Lighting = false;

	vector2di top = cmd->Pos + vector2di(0, (int)cmd->Size);
	vector2di bottom = cmd->Pos - vector2di(0, (int)cmd->Size);
	vector2di right = cmd->Pos + vector2di((int)cmd->Size, 0);
	vector2di left = cmd->Pos - vector2di((int)cmd->Size, 0);

	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());             
	driver->draw2DLine(top, bottom, cmd->Color);
	driver->draw2DLine(right, left, cmd->Color);
}

void DebugDrawManager::drawElem(const DebugDraw_String2D *cmd)
{
	if(gNormalFont14) {
		auto driver = Device->getVideoDriver();
		const irr::core::dimension2du& screenSize = driver->getScreenSize();
		int x = cmd->Pos.X;
		int y = cmd->Pos.Y;
		int w = screenSize.Width;
		int h = screenSize.Height;
		gNormalFont14->draw(cmd->Msg.data(), rect<s32>(x, y, w, h), cmd->Color);
	}
}

void DebugDrawManager::drawElem(const DebugDraw_Circle2D *cmd)
{
	SR_ASSERT(false && "NotImplemented");
}
*/
void DebugDrawManager::drawList(const DebugDrawList_Line3D &cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, core::matrix4());

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		video::SMaterial m; 
		m.Lighting = false;
		m.Thickness = cmd.LineWidthList[i];
		driver->setMaterial(m);
		driver->draw3DLine(cmd.P1List[i], cmd.P2List[i], cmd.ColorList[i]);
	}
}
/*
void DebugDrawManager::drawElem(const DebugDraw_Cross3D *cmd)
{
	//항상 동일한 크기로 보이게 적절히 렌더링하기
}

void DebugDrawManager::drawElem(const DebugDraw_Sphere3D *cmd)
{
	//use scene node based
}
void DebugDrawManager::drawElem(const DebugDraw_String3D *cmd)
{
	//use scene node based
}
void DebugDrawManager::drawElem(const DebugDraw_Axis3D *cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	video::SMaterial m; 
	m.Lighting = false;
	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());             

	vector3df zero(0, 0, 0);
	vector3df x(cmd->Size, 0, 0);
	vector3df y(0, cmd->Size, 0);
	vector3df z(0, 0, cmd->Size);

	cmd->Xf.transformVect(zero);
	cmd->Xf.transformVect(x);
	cmd->Xf.transformVect(y);
	cmd->Xf.transformVect(z);

	SColor red(255, 255, 0, 0);
	SColor green(255, 0, 255, 0);
	SColor blue(255, 0, 0, 255);

	driver->draw3DLine(zero, x, red);
	driver->draw3DLine(zero, y, green);
	driver->draw3DLine(zero, z, blue);
}
*/