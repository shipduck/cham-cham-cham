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


void DebugDrawManager::setUp(irr::IrrlichtDevice *dev)
{
	this->Device = dev;
}
void DebugDrawManager::shutDown()
{
	clear();
}

template<typename TList> struct CountList;
template<>
struct CountList<NullType> {
	static int count(const DebugDrawManager &mgr) { return 0; }
};
template<typename T, typename U>
struct CountList< Typelist<T, U> > {
	static int count(const DebugDrawManager &mgr)
	{
		int sum = 0;
		sum += Field<T>(mgr).getList().size();
		sum += CountList<U>::count(mgr);
		return sum;
	}
};

template<typename TList> struct DrawList;
template<>
struct DrawList<NullType> {
	static void draw(DebugDrawManager &mgr) { return; }
};
template<typename T, typename U>
struct DrawList< Typelist<T, U> > {
	static void draw(DebugDrawManager &mgr)
	{
		auto &elemList = Field<T>(mgr).getList();
		auto it = elemList.begin();
		auto endit = elemList.end();
		for( ; it != endit ; ++it) {
			const T &cmd = *it;
			mgr.drawElem(&cmd);
		}
		DrawList<U>::draw(mgr);
	}
};

template<typename TList> struct ClearList;
template<>
struct ClearList<NullType> {
	static void clear(DebugDrawManager &mgr) { return; }
};
template<typename T, typename U>
struct ClearList< Typelist<T, U> > {
	static void clear(DebugDrawManager &mgr)
	{
		Field<T>(mgr).getList().clear();
		ClearList<U>::clear(mgr);
	}
};

template<typename T>
struct DurationOverFindFunctor {
	bool operator()(T &cmd) {
		return (cmd.Duration < 0);
	}
};

template<typename TList> struct UpdateList;
template<>
struct UpdateList<NullType> {
	static void update(int ms, DebugDrawManager &mgr) { return; }
};
template<typename T, typename U>
struct UpdateList< Typelist<T, U> > {
	static void update(int ms, DebugDrawManager &mgr)
	{
		DurationOverFindFunctor<T> functor;
		auto &elemList = Field<T>(mgr).getList();
		
		auto it = elemList.begin();
		auto endit = elemList.end();
		for( ; it != endit ; ++it) {
			T &cmd = *it;
			cmd.Duration -= ms;

			if(cmd.Duration < 0 && cmd.Node) {
				cmd.Node->remove();
				cmd.Node->drop();
				cmd.Node = nullptr;
			}
		}
		elemList.remove_if(functor);
		
		UpdateList<U>::update(ms, mgr);
	}
};

int DebugDrawManager::count() const
{
	int sum = CountList<DebugDrawCmdTypeList>::count(*this);	
	return sum;
}


void DebugDrawManager::drawAll()
{
	DrawList<DebugDrawCmdTypeList>::draw(*this);
}

void DebugDrawManager::clear() 
{
	ClearList<DebugDrawCmdTypeList>::clear(*this);
}

void DebugDrawManager::updateAll(int ms)
{
	UpdateList<DebugDrawCmdTypeList>::update(ms, *this);
}

void DebugDrawManager::addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration,
		bool depthEnable)
{
	typedef DebugDraw_Line3D CmdType;
	CmdType cmd;
	cmd.P1 = p1;
	cmd.P2 = p2;
	cmd.Color = color;
	cmd.LineWidth = lineWidth;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;
	Field<CmdType>(*this).getList().push_back(cmd);
}

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

void DebugDrawManager::addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration)
{
	typedef DebugDraw_Line2D CmdType;
	CmdType cmd;
	cmd.P1 = p1;
	cmd.P2 = p2;
	cmd.Color = color;
	cmd.LineWidth = lineWidth;
	cmd.Duration = duration;
	Field<CmdType>(*this).getList().push_back(cmd);
}

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

void DebugDrawManager::drawElem(const DebugDraw_Line2D *cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	video::SMaterial m; 
	m.Lighting = false;
	m.Thickness = cmd->LineWidth;
	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());             
	driver->draw2DLine(cmd->P1, cmd->P2, cmd->Color);
}

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

void DebugDrawManager::drawElem(const DebugDraw_Line3D *cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	video::SMaterial m; 
	m.Lighting = false;
	m.Thickness = cmd->LineWidth;
	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());             
	driver->draw3DLine(cmd->P1, cmd->P2, cmd->Color);
}

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