// Ŭnicode please
#pragma once

#include "Typelist.h"
#include "base/template_lib.h"
#include "draw_attribute_list_mixin.h"

class LineBatchSceneNode;

template<typename TList> class DebugDrawListPolicy;

template<>
class DebugDrawListPolicy<Loki::NullType> {
public:
	enum { HasNext = 0 };
	static void clear(GenSimpleHierarchy<Loki::NullType> &obj) {}
	static void pop_back(GenSimpleHierarchy<Loki::NullType> &obj) {}
	static size_t size(const GenSimpleHierarchy<Loki::NullType> &obj) { return 0; }
	static bool validate(const GenSimpleHierarchy<Loki::NullType> &obj) { return true; }
	static void swap(GenSimpleHierarchy<Loki::NullType> &obj, int a, int b) {}
	static void copy_elem(GenSimpleHierarchy<Loki::NullType> &obj,
		GenSimpleHierarchy<Loki::NullType> *target, int idx) {}
};

template<typename T, typename U>
class DebugDrawListPolicy< Loki::Typelist<T, U> > {
public:
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
	static size_t size(const GenSimpleHierarchy< Loki::Typelist<T, U> > &obj)
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
	static void swap(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj, int a, int b)
	{
		static_cast<T&>(obj).swap(a, b);
		Next::swap(obj, a, b);
	}
	static void copy_elem(GenSimpleHierarchy< Loki::Typelist<T, U> > &obj, 
		GenSimpleHierarchy< Loki::Typelist<T, U> > *target, int idx) 
	{
		static_cast<T&>(obj).copy_elem(target, idx);
		Next::copy_elem(obj, target, idx);
	}
};

template<typename TList>
class DebugDrawListT : public GenSimpleHierarchy<TList> {
public:
	typedef DebugDrawListPolicy<TList> Policy;
	void clear() { Policy::clear(*this); }
	void pop_back() { Policy::pop_back(*this); }
	size_t size() const { return ((typename TList::Head*)this)->size(); }
	bool validate() const { return Policy::validate(*this); }
	void swap(int a, int b) { Policy::swap(*this, a, b); }
	void copy_elem(DebugDrawListT *target, int idx) { Policy::copy_elem(*this, target, idx); }
};

typedef TYPELIST_3(
	DrawAttributeListMixin_Line<irr::core::vector2di>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color
	) DrawAttributeListMixin_Line2D;
class DrawAttributeList_Line2D : public DebugDrawListT<DrawAttributeListMixin_Line2D> {};

typedef TYPELIST_4(
	DrawAttributeListMixin_Line<irr::core::vector3df>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_Line3D;
class DrawAttributeList_Line3D : public DebugDrawListT<DrawAttributeListMixin_Line3D> {};

typedef TYPELIST_3(
	DrawAttributeListMixin_Pos<irr::core::vector2di>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color
	) DrawAttributeListMixin_Cross2D;
class DrawAttributeList_Cross2D : public DebugDrawListT<DrawAttributeListMixin_Cross2D> {};

typedef TYPELIST_4(
	DrawAttributeListMixin_Pos<irr::core::vector3df>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_Cross3D;
class DrawAttributeList_Cross3D : public DebugDrawListT<DrawAttributeListMixin_Cross3D> {};

typedef TYPELIST_4(
	DrawAttributeListMixin_Pos<irr::core::vector2di>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_String
	) DrawAttributeListMixin_String2D;
class DrawAttributeList_String2D : public DebugDrawListT<DrawAttributeListMixin_String2D> {};

typedef TYPELIST_6(
	DrawAttributeListMixin_Pos<irr::core::vector3df>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_String,
	DrawAttributeListMixin_3D,
	DrawAttributeListMixin_Node
	) DrawAttributeListMixin_String3D;
class DrawAttributeList_String3D : public DebugDrawListT<DrawAttributeListMixin_String3D> {};

typedef TYPELIST_3(
	DrawAttributeListMixin_Pos<irr::core::vector2di>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color
	) DrawAttributeListMixin_Circle2D;
class DrawAttributeList_Circle2D : public DebugDrawListT<DrawAttributeListMixin_Circle2D> {};

typedef TYPELIST_4(
	DrawAttributeListMixin_Pos<irr::core::vector3df>,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_Color,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_Sphere3D;
class DrawAttributeList_Sphere3D : public DebugDrawListT<DrawAttributeListMixin_Sphere3D> {};

typedef TYPELIST_3(
	DrawAttributeListMixin_Transform,
	DrawAttributeListMixin_Scale<float>,
	DrawAttributeListMixin_3D
	) DrawAttributeListMixin_Axis3D;
class DrawAttributeList_Axis3D : public DebugDrawListT<DrawAttributeListMixin_Axis3D> {};

template<class T>
class DebugDrawListHolder {
public:
	typedef T value_type;

	value_type immediateDrawList;
	value_type durationDrawList;
	std::vector<int> durationList;	//millisecond

	const value_type &getList(int duration) const
	{
		const value_type *drawList = nullptr;
		SR_ASSERT(duration >= 0);
		if(duration == 0) {
			drawList = &immediateDrawList;
		} else {
			drawList = &durationDrawList;
			
		}
		return *drawList;
	}

	value_type &getListAndPushDuration(int duration)
	{
		value_type &drawList = const_cast<value_type&>(getList(duration));
		if(duration > 0) {
			durationList.push_back(duration);
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
			
			if(drawList.size() == durationDrawList.size()) {
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
	DrawAttributeList_Line3D,
	DrawAttributeList_Cross3D,
	DrawAttributeList_Sphere3D,
	DrawAttributeList_Axis3D,
	DrawAttributeList_String3D,
	DrawAttributeList_Line2D,
	DrawAttributeList_Cross2D,
	DrawAttributeList_String2D,
	DrawAttributeList_Circle2D
	) DrawAttributeElemList;

class DebugDrawManager : public Loki::GenScatterHierarchy<DrawAttributeElemList, DebugDrawListHolder> {
public:
	DebugDrawManager() : device_(nullptr), batchSceneNode_(nullptr) {}
	~DebugDrawManager() {}

	void startUp(irr::IrrlichtDevice *dev);
	void shutDown();
	void drawAll();
	void updateAll(int ms);
	void clear();
	size_t size() const;

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
		const DrawAttributeListMixin_String::string_type &msg,
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
		const DrawAttributeListMixin_String::string_type &msg,
		const irr::video::SColor &color,
		float scale = 1.0f,
		int duration = 0);

	void addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration = 0);

	//draw
public:
	void drawList(const DrawAttributeList_Line2D &cmd);
	void drawList(const DrawAttributeList_Cross2D &cmd);
	void drawList(const DrawAttributeList_Circle2D &cmd);
	void drawList(const DrawAttributeList_String2D &cmd);
	void drawList(const DrawAttributeList_Line3D &cmd);
	void drawList(const DrawAttributeList_Cross3D &cmd);
	void drawList(const DrawAttributeList_Sphere3D &cmd);
	void drawList(const DrawAttributeList_Axis3D &cmd);
	void drawList(const DrawAttributeList_String3D &cmd);

private:
	irr::IrrlichtDevice *device_;

private:
	LineBatchSceneNode *batchSceneNode_;
	// thickness != 1 인 경우도 렌더링 로직은 동일하게 처리하기 위해서 도입
	std::map<float, LineBatchSceneNode *> batchSceneNodeMap_;

	LineBatchSceneNode *getBatchSceneNode(float thickness);
};

// 주력으로 사용할것을 전역변수로 걸어놔야 속편하다
extern DebugDrawManager *g_debugDrawMgr;

extern irr::gui::IGUIFont *g_normalFont12;
extern irr::gui::IGUIFont *g_normalFont14;

