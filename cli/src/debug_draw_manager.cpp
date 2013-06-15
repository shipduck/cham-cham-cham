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

void DebugDrawManager::draw()
{
	{
		auto it = Cmd3DList.begin();
		auto endit = Cmd3DList.end();
		for( ; it != endit ; ++it) {
			const std::unique_ptr<DebugDraw3D> &cmd = *it;
			drawElem(cmd.get());
		}
	}
	{
		auto it = Cmd2DList.begin();
		auto endit = Cmd2DList.end();
		for( ; it != endit ; ++it) {
			const std::unique_ptr<DebugDraw2D> &cmd = *it;
			drawElem(cmd.get());
		}
	}
}
void DebugDrawManager::clear() 
{
	Cmd2DList.clear(); 
	Cmd3DList.clear(); 
}


template<typename T>
struct RemoveFindFunctor {
	bool operator()(std::unique_ptr<T> &cmd) {
		return (cmd->Duration < 0);
	}
};

void DebugDrawManager::update(int ms)
{
	{
		RemoveFindFunctor<DebugDraw2D> functor;
		auto it = Cmd2DList.begin();
		auto endit = Cmd2DList.end();
		for( ; it != endit ; ++it) {
			auto &cmd = *it;
			cmd->Duration -= ms;
		}
		Cmd2DList.remove_if(functor);
	}
	{
		RemoveFindFunctor<DebugDraw3D> functor;
		auto it = Cmd3DList.begin();
		auto endit = Cmd3DList.end();
		for( ; it != endit ; ++it) {
			auto &cmd = *it;
			cmd->Duration -= ms;
		}
		Cmd3DList.remove_if(functor);
	}
}

DebugDraw3D::DebugDraw3D(DebugDraw3DType type)
	: Type(type),
	Duration(0),
	DepthEnable(true)
{
}
DebugDraw3D::~DebugDraw3D()
{
}

DebugDraw3D_Sphere::~DebugDraw3D_Sphere()
{
	if(Node) {
		Node->remove();
		Node->drop();
		Node = nullptr;
	}
}

void DebugDrawManager::addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration,
		bool depthEnable)
{
	std::unique_ptr<DebugDraw3D_Line> cmd(new DebugDraw3D_Line());
	cmd->P1 = p1;
	cmd->P2 = p2;
	cmd->Color = color;
	cmd->LineWidth = lineWidth;
	cmd->Duration = duration;
	cmd->DepthEnable = depthEnable;
	Cmd3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addCross(const irr::core::vector3df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration,
		bool depthEnable)
{
	std::unique_ptr<DebugDraw3D_Cross> cmd(new DebugDraw3D_Cross());
	cmd->Pos = pos;
	cmd->Color = color;
	cmd->Size = size;
	cmd->Duration = duration;
	cmd->DepthEnable = depthEnable;
	Cmd3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addSphere(const irr::core::vector3df &pos, 
		float radius,
		const irr::video::SColor &color,
		int duration,
		bool depthEnable)
{
	std::unique_ptr<DebugDraw3D_Sphere> cmd(new DebugDraw3D_Sphere());
	cmd->Pos = pos;
	cmd->Radius = radius;
	cmd->Color = color;
	cmd->Duration = duration;
	cmd->DepthEnable = depthEnable;

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
	cmd->Node = node;

	Cmd3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	std::unique_ptr<DebugDraw3D_Axis> cmd(new DebugDraw3D_Axis());
	cmd->Xf = xf;
	cmd->Size = size;
	cmd->Duration = duration;
	cmd->DepthEnable = depthEnable;
	Cmd3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addString(const irr::core::vector3df &pos, 
		const DebugDrawManager::string_type &msg,
		const irr::video::SColor &color,
		float scale,
		int duration,
		bool depthEnable)
{
	std::unique_ptr<DebugDraw3D_String> cmd(new DebugDraw3D_String());
	cmd->Pos = pos;
	cmd->Msg = msg;
	cmd->Color = color;
	cmd->Scale = scale;
	cmd->Duration = duration;
	cmd->DepthEnable = depthEnable;
	Cmd3DList.push_back(std::move(cmd));
}

void DebugDrawManager::addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration)
{
	std::unique_ptr<DebugDraw2D_Line> cmd(new DebugDraw2D_Line());
	cmd->P1 = p1;
	cmd->P2 = p2;
	cmd->Color = color;
	cmd->LineWidth = lineWidth;
	cmd->Duration = duration;
	Cmd2DList.push_back(std::move(cmd));
}

void DebugDrawManager::addCross(const irr::core::vector2di &pos, 
		const irr::video::SColor &color,
		float size,
		int duration)
{
	std::unique_ptr<DebugDraw2D_Cross> cmd(new DebugDraw2D_Cross());
	cmd->Pos = pos;
	cmd->Color = color;
	cmd->Size = size;
	cmd->Duration = duration;
	Cmd2DList.push_back(std::move(cmd));
}

void DebugDrawManager::addString(const irr::core::vector2di &pos, 
								   const DebugDrawManager::string_type &msg,
								   const irr::video::SColor &color,
								   float scale,
								   int duration)
{
	std::unique_ptr<DebugDraw2D_String> cmd(new DebugDraw2D_String());
	cmd->Pos = pos;
	cmd->Msg = msg;
	cmd->Color = color;
	cmd->Scale = scale;
	cmd->Duration = duration;
	Cmd2DList.push_back(std::move(cmd));
}

void DebugDrawManager::addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration)
{
	std::unique_ptr<DebugDraw2D_Circle> cmd(new DebugDraw2D_Circle());
	cmd->Pos = pos;
	cmd->Radius = radius;
	cmd->Color = color;
	cmd->Duration = duration;
	Cmd2DList.push_back(std::move(cmd));
}


void DebugDrawManager::drawElem(DebugDraw2D *cmd) 
{
	switch(cmd->Type) {
	case kDebugDraw2DLine:
		drawElem(static_cast<DebugDraw2D_Line*>(cmd));
		break;
	case kDebugDraw2DCross:
		drawElem(static_cast<DebugDraw2D_Cross*>(cmd));
		break;
	case kDebugDraw2DString:
		drawElem(static_cast<DebugDraw2D_String*>(cmd));
		break;
	case kDebugDraw2DCircle:
		drawElem(static_cast<DebugDraw2D_Circle*>(cmd));
		break;
	default:
		break;
	}
}


void DebugDrawManager::drawElem(DebugDraw2D_Line *cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	video::SMaterial m; 
	m.Lighting = false;
	m.Thickness = cmd->LineWidth;
	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());             
	driver->draw2DLine(cmd->P1, cmd->P2, cmd->Color);
}

void DebugDrawManager::drawElem(DebugDraw2D_Cross *cmd)
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

void DebugDrawManager::drawElem(DebugDraw2D_String *cmd)
{
}

void DebugDrawManager::drawElem(DebugDraw2D_Circle *cmd)
{
	SR_ASSERT(false && "NotImplemented");
}

void DebugDrawManager::drawElem(DebugDraw3D *cmd) 
{
	switch(cmd->Type) {
	case kDebugDraw3DLine:
		drawElem(static_cast<DebugDraw3D_Line*>(cmd));
		break;
	case kDebugDraw3DCross:
		drawElem(static_cast<DebugDraw3D_Cross*>(cmd));
		break;
	case kDebugDraw3DSphere:
		drawElem(static_cast<DebugDraw3D_Sphere*>(cmd));
		break;
	case kDebugDraw3DString:
		drawElem(static_cast<DebugDraw3D_String*>(cmd));
		break;
	case kDebugDraw3DAxis:
		drawElem(static_cast<DebugDraw3D_Axis*>(cmd));
		break;
	default:
		break;
	}
}

void DebugDrawManager::drawElem(DebugDraw3D_Line *cmd)
{
	IVideoDriver* driver = Device->getVideoDriver();
	video::SMaterial m; 
	m.Lighting = false;
	m.Thickness = cmd->LineWidth;
	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());             
	driver->draw3DLine(cmd->P1, cmd->P2, cmd->Color);
}

void DebugDrawManager::drawElem(DebugDraw3D_Cross *cmd)
{
	//항상 동일한 크기로 보이게 적절히 렌더링하기
}

void DebugDrawManager::drawElem(DebugDraw3D_Sphere *cmd)
{
	//use scene node based
}
void DebugDrawManager::drawElem(DebugDraw3D_String *cmd)
{
}
void DebugDrawManager::drawElem(DebugDraw3D_Axis *cmd)
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