// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_manager.h"

DebugDrawManager3D::DebugDrawManager3D()
	: Device(nullptr)
{
}

DebugDrawManager3D::~DebugDrawManager3D()
{
}

void DebugDrawManager3D::draw()
{
}

void DebugDrawManager3D::update(int ms)
{
	struct RemoveFindFunctor {
		bool operator()(std::unique_ptr<DebugDraw3D> &cmd) {
			return (cmd->Duration < 0);
		}
	};
	RemoveFindFunctor functor;
	
	auto it = CmdList.begin();
	auto endit = CmdList.end();
	for( ; it != endit ; ++it) {
		std::unique_ptr<DebugDraw3D> &cmd = *it;
		cmd->Duration -= ms;
	}
	CmdList.remove_if(functor);
}

void DebugDrawManager3D::setUp(irr::IrrlichtDevice *dev)
{
	this->Device = dev;
}

void DebugDrawManager3D::shutDown()
{
	clear();
}

DebugDraw3D::DebugDraw3D(DebugDraw3DType type)
	: Type(type),
	Duration(0),
	DepthEnable(true) 
{
	//gl::RenderState *render_state = gl::RenderState::Get();
	//proj_mat = render_state->proj_mat;
	//view_mat = render_state->view_mat;
	//model_mat = render_state->model_mat;
}

void DebugDrawManager3D::AddLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
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
	CmdList.push_back(std::move(cmd));
}

void DebugDrawManager3D::AddCross(const irr::core::vector3df &pos, 
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
	CmdList.push_back(std::move(cmd));
}

void DebugDrawManager3D::AddSphere(const irr::core::vector3df &pos, 
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
	CmdList.push_back(std::move(cmd));
}

void DebugDrawManager3D::AddAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	std::unique_ptr<DebugDraw3D_Axis> cmd(new DebugDraw3D_Axis());
	cmd->Xf = xf;
	cmd->Size = size;
	cmd->Duration = duration;
	cmd->DepthEnable = depthEnable;
	CmdList.push_back(std::move(cmd));
}

void DebugDrawManager3D::AddString(const irr::core::vector3df &pos, 
		const std::u32string &msg,
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
	CmdList.push_back(std::move(cmd));
}



DebugDrawManager2D::DebugDrawManager2D()
	: Device(nullptr)
{
}

DebugDrawManager2D::~DebugDrawManager2D()
{
}

void DebugDrawManager2D::setUp(irr::IrrlichtDevice *dev)
{
	this->Device = dev;
}
void DebugDrawManager2D::shutDown()
{
	clear();
}
void DebugDrawManager2D::draw()
{
}
void DebugDrawManager2D::update(int ms)
{
	struct RemoveFindFunctor {
		bool operator()(std::unique_ptr<DebugDraw2D> &cmd) {
			return (cmd->Duration < 0);
		}
	};
	RemoveFindFunctor functor;

	auto it = CmdList.begin();
	auto endit = CmdList.end();
	for( ; it != endit ; ++it) {
		std::unique_ptr<DebugDraw2D> &cmd = *it;
		cmd->Duration -= ms;
	}
	CmdList.remove_if(functor);
}

void DebugDrawManager2D::AddLine(const irr::core::vector2df &p1, const irr::core::vector2df &p2,
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
	CmdList.push_back(std::move(cmd));
}

void DebugDrawManager2D::AddCross(const irr::core::vector2df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration)
{
	std::unique_ptr<DebugDraw2D_Cross> cmd(new DebugDraw2D_Cross());
	cmd->Pos = pos;
	cmd->Color = color;
	cmd->Size = size;
	cmd->Duration = duration;
	CmdList.push_back(std::move(cmd));
}

void DebugDrawManager2D::AddString(const irr::core::vector2df &pos, const std::u32string &msg,
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
	CmdList.push_back(std::move(cmd));
}

void DebugDrawManager2D::AddCircle(const irr::core::vector2df &pos, float radius,
		const irr::video::SColor &color,
		int duration)
{
	std::unique_ptr<DebugDraw2D_Circle> cmd(new DebugDraw2D_Circle());
	cmd->Pos = pos;
	cmd->Radius = radius;
	cmd->Color = color;
	cmd->Duration = duration;
	CmdList.push_back(std::move(cmd));
}
