﻿// Ŭnicode please b
#pragma once

#include "Typelist.h"

typedef enum {
	kDebugDrawNone = -1,
	kDebugDrawLine2D,
	kDebugDrawCross2D,
	kDebugDrawString2D,
	kDebugDrawCircle2D,
	kDebugDrawLine3D,
	kDebugDrawCross3D,
	kDebugDrawSphere3D,
	kDebugDrawAxis3D,
	kDebugDrawString3D,
	kDebugDrawCount,
} DebugDrawType;

struct DebugDraw;
struct DebugDraw_Line3D;
struct DebugDraw_Cross3D;
struct DebugDraw_Sphere3D;
struct DebugDraw_String3D;
struct DebugDraw_Axis3D;
struct DebugDraw_Line2D;
struct DebugDraw_Cross2D;
struct DebugDraw_String2D;
struct DebugDraw_Circle2D;

// 로직 돌릴 순서대로 쓰기
typedef TYPELIST_9(
	DebugDraw_Line3D,
	DebugDraw_Cross3D,
	DebugDraw_Sphere3D,
	DebugDraw_String3D,
	DebugDraw_Axis3D,
	DebugDraw_Line2D,
	DebugDraw_Cross2D,
	DebugDraw_String2D,
	DebugDraw_Circle2D) DebugDrawCmdTypeList;
static_assert(kDebugDrawCount == Loki::TL::Length<DebugDrawCmdTypeList>::value, "type list - type enum not same"); 

struct DebugDraw {
	typedef std::wstring string_type;
	typedef std::wstring::value_type value_type;

	DebugDraw() : Type(kDebugDrawNone), Duration(0), Node(nullptr) {}
	DebugDraw(DebugDrawType type)  : Type(type), Duration(0), Node(nullptr) {}
	virtual ~DebugDraw() {}
	DebugDrawType Type;
	irr::video::SColor Color;
	int Duration;	//millisecond
	irr::scene::ISceneNode *Node;
};

struct DebugDraw3D : public DebugDraw {
	DebugDraw3D() : DebugDraw() {}
	DebugDraw3D(DebugDrawType type)  : DebugDraw(type), DepthEnable(true) {}
	virtual ~DebugDraw3D() {}
	bool DepthEnable;
};

struct DebugDraw_Line2D : public DebugDraw {
	DebugDraw_Line2D() : DebugDraw(kDebugDrawLine2D), LineWidth(1.0f) {}
	virtual ~DebugDraw_Line2D() {}
	irr::core::vector2di P1;
	irr::core::vector2di P2;
	float LineWidth;
};
struct DebugDraw_Cross2D : public DebugDraw {
	DebugDraw_Cross2D() : DebugDraw(kDebugDrawCross2D), Size(5.0f) {}
	virtual ~DebugDraw_Cross2D() {}
	irr::core::vector2di Pos;
	float Size;
};

struct DebugDraw_String2D : public DebugDraw {
	DebugDraw_String2D() : DebugDraw(kDebugDrawString2D), Scale(1.0f) {}
	virtual ~DebugDraw_String2D() {}
	irr::core::vector2di Pos;
	DebugDraw::string_type Msg;
	float Scale;
};
struct DebugDraw_Circle2D: public DebugDraw {
	DebugDraw_Circle2D() : DebugDraw(kDebugDrawCircle2D), Radius(1.0f) {}
	virtual ~DebugDraw_Circle2D() {}
	float Radius;
	irr::core::vector2di Pos;  //cross, sphere, string
};

struct DebugDraw_Line3D : public DebugDraw3D {
	DebugDraw_Line3D() : DebugDraw3D(kDebugDrawLine3D), LineWidth(1.0f) {}
	virtual ~DebugDraw_Line3D() {}
	float LineWidth;
	irr::core::vector3df P1;
	irr::core::vector3df P2;
};

struct DebugDraw_Cross3D : public DebugDraw3D {
	DebugDraw_Cross3D() : DebugDraw3D(kDebugDrawCross3D), Size(1.0f) {}
	virtual ~DebugDraw_Cross3D() {}
	float Size;
	irr::core::vector3df Pos;  //cross
	
};
struct DebugDraw_Sphere3D : public DebugDraw3D {
	DebugDraw_Sphere3D() : DebugDraw3D(kDebugDrawSphere3D), Radius(1.0f) {}
	virtual ~DebugDraw_Sphere3D() {}
	float Radius;
	irr::core::vector3df Pos;  //sphere
};

struct DebugDraw_String3D : public DebugDraw3D {
	DebugDraw_String3D() : DebugDraw3D(kDebugDrawString3D), Scale(1.0f) {}
	virtual ~DebugDraw_String3D() {}
	DebugDraw::string_type Msg;
	float Scale;
	irr::core::vector3df Pos;  //string
};
struct DebugDraw_Axis3D : public DebugDraw3D {
	DebugDraw_Axis3D() : DebugDraw3D(kDebugDrawAxis3D), Size(1.0f) {}
	virtual ~DebugDraw_Axis3D() {}
	float Size;
	irr::core::matrix4 Xf;   //axis
};


template<class T>
class DebugDrawListHolder {
public:
	typedef std::list<T> list_type;
	typedef T value_type;

	list_type &getList() {
		return ListValue; 
	}
	const list_type &getList() const { 
		return ListValue; 
	}
private:
	list_type ListValue;
};

class DebugDrawManager : public Loki::GenScatterHierarchy<DebugDrawCmdTypeList, DebugDrawListHolder> {
public:
	typedef std::list<std::unique_ptr<DebugDraw>> DebugDrawList;

public:
	DebugDrawManager() : Device(nullptr) {}
	~DebugDrawManager() {}

	void setUp(irr::IrrlichtDevice *dev);
	void shutDown();
	void drawAll();
	void updateAll(int ms);
	void clear();
	int count() const;

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
		const DebugDraw::string_type &msg,
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
		const DebugDraw::string_type &msg,
		const irr::video::SColor &color,
		float scale = 1.0f,
		int duration = 0);

	void addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration = 0);

	//draw
public:
	void drawElem(const DebugDraw_Line2D *cmd);
	void drawElem(const DebugDraw_Cross2D *cmd);
	void drawElem(const DebugDraw_Circle2D *cmd);
	void drawElem(const DebugDraw_String2D *cmd);
	void drawElem(const DebugDraw_Line3D *cmd);
	void drawElem(const DebugDraw_Cross3D *cmd);
	void drawElem(const DebugDraw_Sphere3D *cmd);
	void drawElem(const DebugDraw_Axis3D *cmd);
	void drawElem(const DebugDraw_String3D *cmd);

private:
	irr::IrrlichtDevice *Device;	
};

// 주력으로 사용할것을 전역변수로 걸어놔야 속편하다
extern DebugDrawManager gDebugDrawMgr;

extern irr::gui::IGUIFont *gNormalFont12;
extern irr::gui::IGUIFont *gNormalFont14;
