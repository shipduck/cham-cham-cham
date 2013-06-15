// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_manager.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace core;

DebugDrawManager gDebugDrawMgr;
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

int DebugDrawManager::count() const
{
	int sum = 0;
	sum += Line2DList.size();
	sum += Cross2DList.size();
	sum += Circle2DList.size();
	sum += String2DList.size();
	sum += Line3DList.size();
	sum += Cross3DList.size();
	sum += Sphere3DList.size();
	sum += Axis3DList.size();
	sum += String3DList.size();
	return sum;
}

void DebugDrawManager::drawAll()
{
	draw(Line2DList);
	draw(Cross2DList);
	draw(Circle2DList);
	draw(String2DList);
	draw(Line3DList);
	draw(Cross3DList);
	draw(Sphere3DList);
	draw(Axis3DList);
	draw(String3DList);
}
void DebugDrawManager::clear() 
{
	Line2DList.clear();
	Cross2DList.clear();
	Circle2DList.clear();
	String2DList.clear();
	Line3DList.clear();
	Cross3DList.clear();
	Sphere3DList.clear();
	Axis3DList.clear();
	String3DList.clear();
}

void DebugDrawManager::updateAll(int ms)
{
	update(ms, Line2DList);
	update(ms, Cross2DList);
	update(ms, Circle2DList);
	update(ms, String2DList);
	update(ms, Line3DList);
	update(ms, Cross3DList);
	update(ms, Sphere3DList);
	update(ms, Axis3DList);
	update(ms, String3DList);
}

void DebugDrawManager::addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration,
		bool depthEnable)
{
	DebugDraw_Line3D cmd;
	cmd.P1 = p1;
	cmd.P2 = p2;
	cmd.Color = color;
	cmd.LineWidth = lineWidth;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;
	Line3DList.push_back(cmd);
}

void DebugDrawManager::addCross(const irr::core::vector3df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration,
		bool depthEnable)
{
	DebugDraw_Cross3D cmd;
	cmd.Pos = pos;
	cmd.Color = color;
	cmd.Size = size;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;
	Cross3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addSphere(const irr::core::vector3df &pos, 
		float radius,
		const irr::video::SColor &color,
		int duration,
		bool depthEnable)
{
	DebugDraw_Sphere3D cmd;
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

	Sphere3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	DebugDraw_Axis3D cmd;
	cmd.Xf = xf;
	cmd.Size = size;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;
	Axis3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addString(const irr::core::vector3df &pos, 
		const DebugDrawManager::string_type &msg,
		const irr::video::SColor &color,
		float scale,
		int duration,
		bool depthEnable)
{
	DebugDraw_String3D cmd;
	cmd.Pos = pos;
	cmd.Msg = msg;
	cmd.Color = color;
	cmd.Scale = scale;
	cmd.Duration = duration;
	cmd.DepthEnable = depthEnable;
	String3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration)
{
	DebugDraw_Line2D cmd;
	cmd.P1 = p1;
	cmd.P2 = p2;
	cmd.Color = color;
	cmd.LineWidth = lineWidth;
	cmd.Duration = duration;
	Line2DList.push_back(std::move(cmd));
}

void DebugDrawManager::addCross(const irr::core::vector2di &pos, 
		const irr::video::SColor &color,
		float size,
		int duration)
{
	DebugDraw_Cross2D cmd;
	cmd.Pos = pos;
	cmd.Color = color;
	cmd.Size = size;
	cmd.Duration = duration;
	Cross2DList.push_back(std::move(cmd));
}

void DebugDrawManager::addString(const irr::core::vector2di &pos, 
								   const DebugDrawManager::string_type &msg,
								   const irr::video::SColor &color,
								   float scale,
								   int duration)
{
	DebugDraw_String2D cmd;
	cmd.Pos = pos;
	cmd.Msg = msg;
	cmd.Color = color;
	cmd.Scale = scale;
	cmd.Duration = duration;
	String2DList.push_back(std::move(cmd));
}

void DebugDrawManager::addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration)
{
	DebugDraw_Circle2D cmd;
	cmd.Pos = pos;
	cmd.Radius = radius;
	cmd.Color = color;
	cmd.Duration = duration;
	Circle2DList.push_back(std::move(cmd));
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