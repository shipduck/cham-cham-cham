// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_manager.h"

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

void DebugDrawManager3D::addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
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

void DebugDrawManager3D::addCross(const irr::core::vector3df &pos, 
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

void DebugDrawManager3D::addSphere(const irr::core::vector3df &pos, 
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

void DebugDrawManager3D::addAxis(const irr::core::matrix4 &xf,
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

void DebugDrawManager3D::addString(const irr::core::vector3df &pos, 
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

void DebugDrawManager2D::addLine(const irr::core::vector2df &p1, const irr::core::vector2df &p2,
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

void DebugDrawManager2D::addCross(const irr::core::vector2df &pos, 
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

void DebugDrawManager2D::addString(const irr::core::vector2df &pos, const std::u32string &msg,
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

void DebugDrawManager2D::addCircle(const irr::core::vector2df &pos, float radius,
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


void DebugDrawer2D::drawElem(DebugDraw2D_Line *cmd)
{
}

void DebugDrawer2D::drawElem(DebugDraw2D_Cross *cmd)
{
}

void DebugDrawer2D::drawElem(DebugDraw2D_String *cmd)
{
}

void DebugDrawer2D::drawElem(DebugDraw2D_Circle *cmd)
{
}

void DebugDrawer3D::drawElem(DebugDraw3D_Line *cmd)
{
}

void DebugDrawer3D::drawElem(DebugDraw3D_Cross *cmd)
{
}

void DebugDrawer3D::drawElem(DebugDraw3D_Sphere *cmd)
{
}
void DebugDrawer3D::drawElem(DebugDraw3D_String *cmd)
{
}
void DebugDrawer3D::drawElem(DebugDraw3D_Axis *cmd)
{
}