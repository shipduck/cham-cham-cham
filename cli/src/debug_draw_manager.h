// Ŭnicode please b
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

class DebugDrawManager {
public:
	typedef std::wstring string_type;
	typedef std::wstring::value_type value_type;

public:
	typedef std::list<std::unique_ptr<DebugDraw2D>> DebugDraw2DList;
	typedef std::list<std::unique_ptr<DebugDraw3D>> DebugDraw3DList;

public:
	DebugDrawManager() : Device(nullptr) {}
	~DebugDrawManager() {}

	void setUp(irr::IrrlichtDevice *dev);
	void shutDown();
	void draw();
	void update(int ms);
	void clear();
	int count2d() const { return Cmd2DList.size(); }
	int count3d() const { return Cmd3DList.size(); }

	//add 3d
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
		const DebugDrawManager::string_type &msg,
		const irr::video::SColor &color,
		float scale = 1.0f,
		int duration = 0,
		bool depthEnable = true);

	//add 2d
public:
	void addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth = 1.0f,
		int duration = 0);

	void addCross(const irr::core::vector2di &pos, 
		const irr::video::SColor &color,
		float size,
		int duration = 0);

	void addString(const irr::core::vector2di &pos, 
		const DebugDrawManager::string_type &msg,
		const irr::video::SColor &color,
		float scale = 1.0f,
		int duration = 0);

	void addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration = 0);

	//draw 2d
public:
	void drawElem(DebugDraw2D *cmd);
	void drawElem(DebugDraw2D_Line *cmd);
	void drawElem(DebugDraw2D_Cross *cmd);
	void drawElem(DebugDraw2D_Circle *cmd);
	void drawElem(DebugDraw2D_String *cmd);

	//draw 3d
public:
	void drawElem(DebugDraw3D *cmd);
	void drawElem(DebugDraw3D_Line *cmd);
	void drawElem(DebugDraw3D_Cross *cmd);
	void drawElem(DebugDraw3D_Sphere *cmd);
	void drawElem(DebugDraw3D_Axis *cmd);
	void drawElem(DebugDraw3D_String *cmd);

protected:
	irr::IrrlichtDevice *Device;	
	DebugDraw2DList Cmd2DList;
	DebugDraw3DList Cmd3DList;
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
	irr::core::vector2di P1;
	irr::core::vector2di P2;
	float LineWidth;
};
struct DebugDraw2D_Cross : public DebugDraw2D {
	DebugDraw2D_Cross() : DebugDraw2D(kDebugDraw2DCross), Size(5.0f) {}
	virtual ~DebugDraw2D_Cross() {}
	irr::core::vector2di Pos;
	float Size;
};

struct DebugDraw2D_String : public DebugDraw2D {
	DebugDraw2D_String() : DebugDraw2D(kDebugDraw2DString), Scale(1.0f) {}
	virtual ~DebugDraw2D_String() {}
	irr::core::vector2di Pos;
	DebugDrawManager::string_type Msg;
	float Scale;
};
struct DebugDraw2D_Circle: public DebugDraw2D {
	DebugDraw2D_Circle() : DebugDraw2D(kDebugDraw2DCircle), Radius(1.0f) {}
	virtual ~DebugDraw2D_Circle() {}
	float Radius;
	irr::core::vector2di Pos;  //cross, sphere, string
};

struct DebugDraw3D {
	DebugDraw3D(DebugDraw3DType type);
	virtual ~DebugDraw3D();

	DebugDraw3DType Type;

	//shared
	irr::video::SColor Color;
	int Duration;	//millisecond
	bool DepthEnable;
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
	DebugDraw3D_Sphere() : DebugDraw3D(kDebugDraw3DSphere), Radius(1.0f), Node(nullptr) {}
	virtual ~DebugDraw3D_Sphere();
	float Radius;
	irr::core::vector3df Pos;  //sphere

	//scene node
	irr::scene::ISceneNode *Node;
};

struct DebugDraw3D_String : public DebugDraw3D {
	DebugDraw3D_String() : DebugDraw3D(kDebugDraw3DString), Scale(1.0f) {}
	DebugDrawManager::string_type Msg;
	float Scale;
	irr::core::vector3df Pos;  //string
};
struct DebugDraw3D_Axis : public DebugDraw3D {
	DebugDraw3D_Axis() : DebugDraw3D(kDebugDraw3DAxis), Size(1.0f) {}
	float Size;
	irr::core::matrix4 Xf;   //axis
};

// 주력으로 사용할것을 전역변수로 걸어놔야 속편하다
extern DebugDrawManager gDebugDrawMgr;

extern irr::gui::IGUIFont *gNormalFont12;
extern irr::gui::IGUIFont *gNormalFont14;