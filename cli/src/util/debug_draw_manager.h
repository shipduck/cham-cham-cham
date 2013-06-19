// Ŭnicode please b
#pragma once

#include "Typelist.h"
#include "base/template_lib.h"

struct DebugDrawList {
	typedef std::wstring string_type;
	typedef std::wstring::value_type value_type;
};

struct DebugDrawListMixin_Color {
	std::vector<irr::video::SColor> ColorList;	
	void clear();
	void pop_back();
	int size() const;
};

struct DebugDrawListMixin_Node {
	std::vector<irr::scene::ISceneNode*> NodeList;
	void clear();
	void pop_back();
	int size() const;
};

struct DebugDrawListMixin_3D {
	std::vector<bool> DepthEnable;
	void clear();
	void pop_back();
	int size() const;
};

struct DebugDrawListMixin_LineWidth {
	std::vector<float> LineWidthList;
	void clear();
	void pop_back();
	int size() const;
};

/*
struct DebugDraw_Cross2D : public DebugDraw {
	DebugDraw_Cross2D() : Size(5.0f) {}
	irr::core::vector2di Pos;
	float Size;
};

struct DebugDraw_String2D : public DebugDraw {
	DebugDraw_String2D() : Scale(1.0f) {}
	irr::core::vector2di Pos;
	DebugDraw::string_type Msg;
	float Scale;
};
struct DebugDraw_Circle2D: public DebugDraw {
	DebugDraw_Circle2D() : Radius(1.0f) {}
	float Radius;
	irr::core::vector2di Pos;  //cross, sphere, string
};
*/

template<typename T>
struct DebugDrawListMixin_Line {
	std::vector<T> P1List;
	std::vector<T> P2List;

	void clear()
	{
		P1List.clear();
		P2List.clear();
	}
	void pop_back()
	{
		P1List.pop_back();
		P2List.pop_back();
	}
	int size() const
	{
		return P1List.size();
	}
};

template<typename TList> struct DebugDrawListPolicy;

template<>
struct DebugDrawListPolicy<Loki::NullType> {
	static void clear(GenSimpleHierarchy<Loki::NullType> &obj) {}
	static void pop_back(GenSimpleHierarchy<Loki::NullType> &obj) {}
};

template<typename T, typename U>
struct DebugDrawListPolicy< Loki::Typelist<T, U> > {
	static void clear(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		static_cast<T&>(obj).clear();
		DebugDrawListPolicy<U>::clear(obj);
	};
	static void pop_back(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		static_cast<T&>(obj).pop_back();
		DebugDrawListPolicy<U>::pop_back(obj);
	};
};

template<typename TList>
struct DebugDrawListT : public GenSimpleHierarchy<TList> {
	typedef DebugDrawListPolicy<TList> Policy;
	void clear() { Policy::clear(*this); }
	void pop_back() { Policy::pop_back(*this); }
	int size() const { return ((TList::Head*)this)->size();	}
};

typedef TYPELIST_5(
	DebugDrawListMixin_Color,
	DebugDrawListMixin_Node, 
	DebugDrawListMixin_3D,
	DebugDrawListMixin_LineWidth,
	DebugDrawListMixin_Line<irr::core::vector3df>
	) DebugDrawListMixinList_Line3D;
struct DebugDrawList_Line3D : public DebugDrawListT<DebugDrawListMixinList_Line3D> {};

typedef TYPELIST_3(
	DebugDrawListMixin_Line<irr::core::vector2di>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_LineWidth
	) DebugDrawListMixinList_Line2D;
struct DebugDrawList_Line2D : public DebugDrawListT<DebugDrawListMixinList_Line2D> {};



/*
struct DebugDraw_Cross3D : public DebugDraw3D {
	DebugDraw_Cross3D() : Size(1.0f) {}
	float Size;
	irr::core::vector3df Pos;  //cross
	
};
struct DebugDraw_Sphere3D : public DebugDraw3D {
	DebugDraw_Sphere3D() : Radius(1.0f) {}
	float Radius;
	irr::core::vector3df Pos;  //sphere
};

struct DebugDraw_String3D : public DebugDraw3D {
	DebugDraw_String3D() : Scale(1.0f) {}
	DebugDraw::string_type Msg;
	float Scale;
	irr::core::vector3df Pos;  //string
};
struct DebugDraw_Axis3D : public DebugDraw3D {
	DebugDraw_Axis3D() : Size(1.0f) {}
	float Size;
	irr::core::matrix4 Xf;   //axis
};
*/

template<class T>
class DebugDrawListHolder {
public:
	typedef T value_type;

	value_type ImmediateDrawList;
	value_type DurationDrawList;
	std::vector<int> DurationList;	//millisecond
};


// 로직 돌릴 순서대로 쓰기
typedef TYPELIST_2(
	DebugDrawList_Line3D,
	DebugDrawList_Line2D
	/*
	DebugDraw_Cross3D,
	DebugDraw_Sphere3D,
	DebugDraw_String3D,
	DebugDraw_Axis3D,
	DebugDraw_Line2D,
	DebugDraw_Cross2D,
	DebugDraw_String2D,
	DebugDraw_Circle2D*/) DebugDrawCmdTypeList;

class DebugDrawManager : public Loki::GenScatterHierarchy<DebugDrawCmdTypeList, DebugDrawListHolder> {
public:
	DebugDrawManager() : Device(nullptr) {}
	~DebugDrawManager() {}

	void setUp(irr::IrrlichtDevice *dev);
	void shutDown();
	void drawAll();
	void updateAll(int ms);
	void clear();
	int size() const;

	//add 3d
public:
	void addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth = 1.0f,
		int duration = 0,
		bool depthEnable = true);
	/*
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
	*/
	
	//add 2d
public:
	void addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth = 1.0f,
		int duration = 0);
	/*
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
		*/
	//draw
public:
	void drawList(const DebugDrawList_Line2D &cmd);
	/*
	void drawList(const DebugDraw_Cross2D *cmd);
	void drawList(const DebugDraw_Circle2D *cmd);
	void drawList(const DebugDraw_String2D *cmd);
	*/
	void drawList(const DebugDrawList_Line3D &cmd);
	/*
	void drawList(const DebugDraw_Cross3D *cmd);
	void drawList(const DebugDraw_Sphere3D *cmd);
	void drawList(const DebugDraw_Axis3D *cmd);
	void drawList(const DebugDraw_String3D *cmd);
	*/

private:
	irr::IrrlichtDevice *Device;	
};

// 주력으로 사용할것을 전역변수로 걸어놔야 속편하다
extern DebugDrawManager *gDebugDrawMgr;

extern irr::gui::IGUIFont *gNormalFont12;
extern irr::gui::IGUIFont *gNormalFont14;

