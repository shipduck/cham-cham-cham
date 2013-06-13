// Ŭnicode please 
#pragma once

typedef enum {
	kDebugDraw2DNone,
	kDebugDraw2DLine,
	kDebugDraw2DCross,
	kDebugDraw2DString,
	kDebugDraw2DCircle,
} DebugDraw2DType;

typedef enum {
	kDebugDraw3DNone,
	kDebugDraw3DLine,
	kDebugDraw3DCross,
	kDebugDraw3DSphere,
	kDebugDraw3DAxis,
	kDebugDraw3DString
} DebugDraw3DType;

struct DebugDraw3D;
struct DebugDraw3D_Line;
struct DebugDraw3D_Cross;
struct DebugDraw3D_Sphere;
struct DebugDraw3D_String;
struct DebugDraw3D_Axis;

struct DebugDraw2D;
struct DebugDraw2D_Line;
struct DebugDraw2D_Cross;
struct DebugDraw2D_String;
struct DebugDraw2D_Circle;

template<typename DebugDraw>
class DebugDrawManagerT {
public:
	typedef std::list<std::unique_ptr<DebugDraw>> DebugDrawList;
public:
	DebugDrawManagerT() : Device(nullptr) {}
	~DebugDrawManagerT() {}

	void setUp(irr::IrrlichtDevice *dev)
	{
		this->Device = dev;
	}
	void shutDown()
	{
		clear();
	}

	void draw()
	{
	}
	void update(int ms)
	{
		struct RemoveFindFunctor {
			bool operator()(std::unique_ptr<DebugDraw> &cmd) {
				return (cmd->Duration < 0);
			}
		};
		RemoveFindFunctor functor;

		auto it = CmdList.begin();
		auto endit = CmdList.end();
		for( ; it != endit ; ++it) {
			std::unique_ptr<DebugDraw> &cmd = *it;
			cmd->Duration -= ms;
		}
		CmdList.remove_if(functor);
	}

	void clear() { CmdList.clear(); }
	int count() const { return CmdList.size(); }

protected:
	irr::IrrlichtDevice *Device;	
	DebugDrawList CmdList;
};

class DebugDrawManager3D : public DebugDrawManagerT<DebugDraw3D> {	
public:
	void addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth = 1.0f,
		int duration = 0,
		bool depthEnable = true);

	void addCross(const irr::core::vector3df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration = 0,
		bool depthEnable = true);

	void addSphere(const irr::core::vector3df &pos, 
		float radius,
		const irr::video::SColor &color,
		int duration = 0,
		bool depthEnable = true);

	void addAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration = 0,
		bool depthEnable = true);

	void addString(const irr::core::vector3df &pos, 
		const std::u32string &msg,
		const irr::video::SColor &color,
		float scale = 1.0f,
		int duration = 0,
		bool depthEnable = true);
};

class DebugDrawManager2D : public DebugDrawManagerT<DebugDraw2D> {
public:
	void addLine(const irr::core::vector2df &p1, const irr::core::vector2df &p2,
		const irr::video::SColor &color,
		float lineWidth = 1.0f,
		int duration = 0);

	void addCross(const irr::core::vector2df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration = 0);

	void addString(const irr::core::vector2df &pos, const std::u32string &msg,
		const irr::video::SColor &color,
		float scale = 1.0f,
		int duration = 0);

	void addCircle(const irr::core::vector2df &pos, float radius,
		const irr::video::SColor &color,
		int duration = 0);
};

class DebugDrawer2D {
public:
	void drawElem(DebugDraw2D_Line *cmd);
	void drawElem(DebugDraw2D_Cross *cmd);
	void drawElem(DebugDraw2D_String *cmd);
	void drawElem(DebugDraw2D_Circle *cmd);
};

class DebugDrawer3D {
public:
	void drawElem(DebugDraw3D_Line *cmd);
	void drawElem(DebugDraw3D_Cross *cmd);
	void drawElem(DebugDraw3D_Sphere *cmd);
	void drawElem(DebugDraw3D_String *cmd);
	void drawElem(DebugDraw3D_Axis *cmd);
};


struct DebugDraw2D {
	DebugDraw2D() : Type(kDebugDraw2DNone), Duration(0) {}
	DebugDraw2D(DebugDraw2DType type)  : Type(type), Duration(0) {}
	virtual ~DebugDraw2D() {}
	DebugDraw2DType Type;
	irr::video::SColor Color;
	int Duration;	//millisecond
};
struct DebugDraw2D_Line : public DebugDraw2D {
	DebugDraw2D_Line() : DebugDraw2D(kDebugDraw2DLine), LineWidth(1.0f) {}
	virtual ~DebugDraw2D_Line() {}
	irr::core::vector2df P1;
	irr::core::vector2df P2;
	float LineWidth;
};
struct DebugDraw2D_Cross : public DebugDraw2D {
	DebugDraw2D_Cross() : DebugDraw2D(kDebugDraw2DCross), Size(5.0f) {}
	virtual ~DebugDraw2D_Cross() {}
	irr::core::vector2df Pos;
	float Size;
};
struct DebugDraw2D_String : public DebugDraw2D {
	DebugDraw2D_String() : DebugDraw2D(kDebugDraw2DString), Scale(1.0f) {}
	virtual ~DebugDraw2D_String() {}
	irr::core::vector2df Pos;
	std::u32string Msg;
	float Scale;
};
struct DebugDraw2D_Circle: public DebugDraw2D {
	DebugDraw2D_Circle() : DebugDraw2D(kDebugDraw2DCircle), Radius(1.0f) {}
	virtual ~DebugDraw2D_Circle() {}
	float Radius;
	irr::core::vector2df Pos;  //cross, sphere, string
};

struct DebugDraw3D {
	DebugDraw3D(DebugDraw3DType type);
	virtual ~DebugDraw3D() {}

	DebugDraw3DType Type;

	//shared
	irr::video::SColor Color;
	int Duration;	//millisecond
	bool DepthEnable;

	irr::core::matrix4 ProjMat;
	irr::core::matrix4 ViewMat;
	irr::core::matrix4 ModelMat;

	//glm::mat4 GetMVPMatrix() const { return proj_mat * view_mat * model_mat; }
};

struct DebugDraw3D_Line : public DebugDraw3D {
	DebugDraw3D_Line() : DebugDraw3D(kDebugDraw3DLine), LineWidth(1.0f) {}
	float LineWidth;
	irr::core::vector3df P1;
	irr::core::vector3df P2;
};

struct DebugDraw3D_Cross : public DebugDraw3D {
	DebugDraw3D_Cross() : DebugDraw3D(kDebugDraw3DCross), Size(1.0f) {}
	float Size;
	irr::core::vector3df Pos;  //cross
};
struct DebugDraw3D_Sphere : public DebugDraw3D {
	DebugDraw3D_Sphere() : DebugDraw3D(kDebugDraw3DSphere), Radius(1.0f) {}
	float Radius;
	irr::core::vector3df Pos;  //sphere
};
struct DebugDraw3D_String : public DebugDraw3D {
	DebugDraw3D_String() : DebugDraw3D(kDebugDraw3DString), Scale(1.0f) {}
	std::u32string Msg;
	float Scale;
	irr::core::vector3df Pos;  //string
};
struct DebugDraw3D_Axis : public DebugDraw3D {
	DebugDraw3D_Axis() : DebugDraw3D(kDebugDraw3DAxis), Size(1.0f) {}
	float Size;
	irr::core::matrix4 Xf;   //axis
};
