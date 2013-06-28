// Ŭnicode please b
#pragma once

#include "Typelist.h"
#include "base/template_lib.h"

class LineBatchSceneNode;

struct DebugDrawListMixin_Color {
	std::vector<irr::video::SColor> ColorList;	
	void clear() { ColorList.clear(); }
	void pop_back() { ColorList.pop_back(); }
	int size() const { return ColorList.size(); }
};

struct DebugDrawListMixin_Node {
	~DebugDrawListMixin_Node();
	std::vector<irr::scene::ISceneNode*> NodeList;
	void clear();
	void pop_back();
	int size() const { return NodeList.size(); }
};

struct DebugDrawListMixin_3D {
	std::vector<bool> DepthEnableList;
	void clear() { DepthEnableList.clear(); }
	void pop_back() { DepthEnableList.pop_back(); }
	int size() const { return DepthEnableList.size(); }
};

template<typename T>
struct DebugDrawListMixin_Scale {
	std::vector<T> ScaleList;
	void clear() { ScaleList.clear(); }
	void pop_back() { ScaleList.pop_back(); }
	int size() const { return ScaleList.size(); }
};

struct DebugDrawListMixin_Transform {
	std::vector<irr::core::matrix4> XfList;
	void clear() { XfList.clear(); }
	void pop_back() { XfList.pop_back(); }
	int size() const { return XfList.size(); }
};

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
		SR_ASSERT(P1List.size() == P2List.size());
		return P1List.size();
	}
};

template<typename T>
struct DebugDrawListMixin_Pos {
	std::vector<T> PosList;
	void clear() { PosList.clear(); }
	void pop_back() { PosList.pop_back(); }
	int size() const { return PosList.size(); }
};

struct DebugDrawListMixin_String {
	typedef std::wstring string_type;
	typedef std::wstring::value_type value_type;

	std::vector<string_type> MsgList;

	void clear() { MsgList.clear(); }
	void pop_back() { MsgList.pop_back(); }
	int size() const { return MsgList.size(); }
};

template<typename TList> struct DebugDrawListPolicy;

template<>
struct DebugDrawListPolicy<Loki::NullType> {
	enum { HasNext = 0 };
	static void clear(GenSimpleHierarchy<Loki::NullType> &obj) {}
	static void pop_back(GenSimpleHierarchy<Loki::NullType> &obj) {}
	static int size(const GenSimpleHierarchy<Loki::NullType> &obj) { return 0; }
	static bool validate(const GenSimpleHierarchy<Loki::NullType> &obj) { return true; }
};

template<typename T, typename U>
struct DebugDrawListPolicy< Loki::Typelist<T, U> > {
	enum { HasNext = 1 };
	typedef DebugDrawListPolicy<U> Next;
	static void clear(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		static_cast<T&>(obj).clear();
		Next::clear(obj);
	};
	static void pop_back(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		static_cast<T&>(obj).pop_back();
		Next::pop_back(obj);
	};
	static int size(const GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		return static_cast<const T&>(obj).size();
	}
	static bool validate(const GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
	{
		int leftSize = size(obj);
		int rightSize = Next::size(obj);
		if(Next::HasNext == 1 && leftSize != rightSize) {
			return false;
		}
		return Next::validate(obj);
	}
};

template<typename TList>
struct DebugDrawListT : public GenSimpleHierarchy<TList> {
	typedef DebugDrawListPolicy<TList> Policy;
	void clear() { Policy::clear(*this); }
	void pop_back() { Policy::pop_back(*this); }
	int size() const { return ((typename TList::Head*)this)->size(); }
	bool validate() const { return Policy::validate(*this); }
};

typedef TYPELIST_3(
	DebugDrawListMixin_Line<irr::core::vector2di>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color
	) DebugDrawListMixinList_Line2D;
struct DebugDrawList_Line2D : public DebugDrawListT<DebugDrawListMixinList_Line2D> {};

typedef TYPELIST_4(
	DebugDrawListMixin_Line<irr::core::vector3df>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_3D
	) DebugDrawListMixinList_Line3D;
struct DebugDrawList_Line3D : public DebugDrawListT<DebugDrawListMixinList_Line3D> {};

typedef TYPELIST_3(
	DebugDrawListMixin_Pos<irr::core::vector2di>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color
	) DebugDrawListMixinList_Cross2D;
struct DebugDrawList_Cross2D : public DebugDrawListT<DebugDrawListMixinList_Cross2D> {};

typedef TYPELIST_4(
	DebugDrawListMixin_Pos<irr::core::vector3df>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_3D
	) DebugDrawListMixinList_Cross3D;
struct DebugDrawList_Cross3D : public DebugDrawListT<DebugDrawListMixinList_Cross3D> {};

typedef TYPELIST_4(
	DebugDrawListMixin_Pos<irr::core::vector2di>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_String
	) DebugDrawListMixinList_String2D;
struct DebugDrawList_String2D : public DebugDrawListT<DebugDrawListMixinList_String2D> {};

typedef TYPELIST_6(
	DebugDrawListMixin_Pos<irr::core::vector3df>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_String,
	DebugDrawListMixin_3D,
	DebugDrawListMixin_Node
	) DebugDrawListMixinList_String3D;
struct DebugDrawList_String3D : public DebugDrawListT<DebugDrawListMixinList_String3D> {};

typedef TYPELIST_3(
	DebugDrawListMixin_Pos<irr::core::vector2di>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color
	) DebugDrawListMixinList_Circle2D;
struct DebugDrawList_Circle2D : public DebugDrawListT<DebugDrawListMixinList_Circle2D> {};

typedef TYPELIST_4(
	DebugDrawListMixin_Pos<irr::core::vector3df>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_3D
	) DebugDrawListMixinList_Sphere3D;
struct DebugDrawList_Sphere3D : public DebugDrawListT<DebugDrawListMixinList_Sphere3D> {};

typedef TYPELIST_3(
	DebugDrawListMixin_Transform,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_3D
	) DebugDrawListMixinList_Axis3D;
struct DebugDrawList_Axis3D : public DebugDrawListT<DebugDrawListMixinList_Axis3D> {};

template<class T>
class DebugDrawListHolder {
public:
	typedef T value_type;

	value_type ImmediateDrawList;
	value_type DurationDrawList;
	std::vector<int> DurationList;	//millisecond

	const value_type &getList(int duration) const
	{
		const value_type *drawList = nullptr;
		SR_ASSERT(duration >= 0);
		if(duration == 0) {
			drawList = &ImmediateDrawList;
		} else {
			drawList = &DurationDrawList;
			
		}
		return *drawList;
	}

	value_type &getListAndPushDuration(int duration)
	{
		value_type &drawList = const_cast<value_type&>(getList(duration));
		if(duration > 0) {
			DurationList.push_back(duration);
		}
		return drawList;
	}

	bool runValidateOnce(int duration) const
	{
		static bool immediateValidateRun = false;
		static bool durationValidateRun = false;

		const value_type &drawList = getList(duration);
		if(duration > 0 && durationValidateRun) {
			durationValidateRun = true;
			bool validateResult = drawList.validate();
			SR_ASSERT(validateResult == true);
			
			if(drawList.size() == DurationDrawList.size()) {
				validateResult = false;
			}
			SR_ASSERT(validateResult == true);

			return validateResult;
		} else if(duration == 0 && immediateValidateRun) {
			immediateValidateRun = true;
			bool validateResult = drawList.validate();
			SR_ASSERT(validateResult == true);
			return validateResult;
		} else {
			return true;
		}
	}
};


// 로직 돌릴 순서대로 쓰기
typedef TYPELIST_9(
	DebugDrawList_Line3D,
	DebugDrawList_Cross3D,
	DebugDrawList_Sphere3D,
	DebugDrawList_Axis3D,
	DebugDrawList_String3D,
	DebugDrawList_Line2D,
	DebugDrawList_Cross2D,
	DebugDrawList_String2D,
	DebugDrawList_Circle2D
	) DebugDrawCmdTypeList;

class DebugDrawManager : public Loki::GenScatterHierarchy<DebugDrawCmdTypeList, DebugDrawListHolder> {
public:
	DebugDrawManager() : Device(nullptr), batchSceneNode(nullptr) {}
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
		float size = 1.0f, 
		int duration = 0,
		bool depthEnable = true);

	void addString(const irr::core::vector3df &pos, 
		const DebugDrawListMixin_String::string_type &msg,
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
		const DebugDrawListMixin_String::string_type &msg,
		const irr::video::SColor &color,
		float scale = 1.0f,
		int duration = 0);

	void addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration = 0);

	//draw
public:
	void drawList(const DebugDrawList_Line2D &cmd);
	void drawList(const DebugDrawList_Cross2D &cmd);
	void drawList(const DebugDrawList_Circle2D &cmd);
	void drawList(const DebugDrawList_String2D &cmd);
	void drawList(const DebugDrawList_Line3D &cmd);
	void drawList(const DebugDrawList_Cross3D &cmd);
	void drawList(const DebugDrawList_Sphere3D &cmd);
	void drawList(const DebugDrawList_Axis3D &cmd);
	void drawList(const DebugDrawList_String3D &cmd);

private:
	irr::IrrlichtDevice *Device;
	LineBatchSceneNode *batchSceneNode;
};

// 주력으로 사용할것을 전역변수로 걸어놔야 속편하다
extern DebugDrawManager *gDebugDrawMgr;

extern irr::gui::IGUIFont *gNormalFont12;
extern irr::gui::IGUIFont *gNormalFont14;

