// Ŭnicode please b
#pragma once

#include "Typelist.h"
#include "base/template_lib.h"

class LineBatchSceneNode;

class DebugDrawListMixin_Color {
public:
	std::vector<irr::video::SColor> colorList;	
	void clear() { colorList.clear(); }
	void pop_back() { colorList.pop_back(); }
	size_t size() const { return colorList.size(); }
};

class DebugDrawListMixin_Node {
public:
	~DebugDrawListMixin_Node();
	std::vector<irr::scene::ISceneNode*> nodeList;
	void clear();
	void pop_back();
	size_t size() const { return nodeList.size(); }
};

class DebugDrawListMixin_3D {
public:
	std::vector<bool> depthEnableList;
	void clear() { depthEnableList.clear(); }
	void pop_back() { depthEnableList.pop_back(); }
	size_t size() const { return depthEnableList.size(); }
};

template<typename T>
class DebugDrawListMixin_Scale {
public:
	std::vector<T> scaleList;
	void clear() { scaleList.clear(); }
	void pop_back() { scaleList.pop_back(); }
	size_t size() const { return scaleList.size(); }
};

class DebugDrawListMixin_Transform {
public:
	std::vector<irr::core::matrix4> xfList;
	void clear() { xfList.clear(); }
	void pop_back() { xfList.pop_back(); }
	size_t size() const { return xfList.size(); }
};

template<typename T>
class DebugDrawListMixin_Line {
public:
	std::vector<T> p1List;
	std::vector<T> p2List;

	void clear()
	{
		p1List.clear();
		p2List.clear();
	}
	void pop_back()
	{
		p1List.pop_back();
		p2List.pop_back();
	}
	size_t size() const
	{
		SR_ASSERT(p1List.size() == p2List.size());
		return p1List.size();
	}
};

template<typename T>
class DebugDrawListMixin_Pos {
public:
	std::vector<T> posList;
	void clear() { posList.clear(); }
	void pop_back() { posList.pop_back(); }
	size_t size() const { return posList.size(); }
};

class DebugDrawListMixin_String {
public:
	typedef std::wstring string_type;
	typedef std::wstring::value_type value_type;

	std::vector<string_type> msgList;

	void clear() { msgList.clear(); }
	void pop_back() { msgList.pop_back(); }
	size_t size() const { return msgList.size(); }
};

template<typename TList> class DebugDrawListPolicy;

template<>
class DebugDrawListPolicy<Loki::NullType> {
public:
	enum { HasNext = 0 };
	static void clear(GenSimpleHierarchy<Loki::NullType> &obj) {}
	static void pop_back(GenSimpleHierarchy<Loki::NullType> &obj) {}
	static size_t size(const GenSimpleHierarchy<Loki::NullType> &obj) { return 0; }
	static bool validate(const GenSimpleHierarchy<Loki::NullType> &obj) { return true; }
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
};

template<typename TList>
class DebugDrawListT : public GenSimpleHierarchy<TList> {
public:
	typedef DebugDrawListPolicy<TList> Policy;
	void clear() { Policy::clear(*this); }
	void pop_back() { Policy::pop_back(*this); }
	size_t size() const { return ((typename TList::Head*)this)->size(); }
	bool validate() const { return Policy::validate(*this); }
};

typedef TYPELIST_3(
	DebugDrawListMixin_Line<irr::core::vector2di>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color
	) DebugDrawListMixinList_Line2D;
class DebugDrawList_Line2D : public DebugDrawListT<DebugDrawListMixinList_Line2D> {};

typedef TYPELIST_4(
	DebugDrawListMixin_Line<irr::core::vector3df>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_3D
	) DebugDrawListMixinList_Line3D;
class DebugDrawList_Line3D : public DebugDrawListT<DebugDrawListMixinList_Line3D> {};

typedef TYPELIST_3(
	DebugDrawListMixin_Pos<irr::core::vector2di>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color
	) DebugDrawListMixinList_Cross2D;
class DebugDrawList_Cross2D : public DebugDrawListT<DebugDrawListMixinList_Cross2D> {};

typedef TYPELIST_4(
	DebugDrawListMixin_Pos<irr::core::vector3df>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_3D
	) DebugDrawListMixinList_Cross3D;
class DebugDrawList_Cross3D : public DebugDrawListT<DebugDrawListMixinList_Cross3D> {};

typedef TYPELIST_4(
	DebugDrawListMixin_Pos<irr::core::vector2di>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_String
	) DebugDrawListMixinList_String2D;
class DebugDrawList_String2D : public DebugDrawListT<DebugDrawListMixinList_String2D> {};

typedef TYPELIST_6(
	DebugDrawListMixin_Pos<irr::core::vector3df>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_String,
	DebugDrawListMixin_3D,
	DebugDrawListMixin_Node
	) DebugDrawListMixinList_String3D;
class DebugDrawList_String3D : public DebugDrawListT<DebugDrawListMixinList_String3D> {};

typedef TYPELIST_3(
	DebugDrawListMixin_Pos<irr::core::vector2di>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color
	) DebugDrawListMixinList_Circle2D;
class DebugDrawList_Circle2D : public DebugDrawListT<DebugDrawListMixinList_Circle2D> {};

typedef TYPELIST_4(
	DebugDrawListMixin_Pos<irr::core::vector3df>,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_Color,
	DebugDrawListMixin_3D
	) DebugDrawListMixinList_Sphere3D;
class DebugDrawList_Sphere3D : public DebugDrawListT<DebugDrawListMixinList_Sphere3D> {};

typedef TYPELIST_3(
	DebugDrawListMixin_Transform,
	DebugDrawListMixin_Scale<float>,
	DebugDrawListMixin_3D
	) DebugDrawListMixinList_Axis3D;
class DebugDrawList_Axis3D : public DebugDrawListT<DebugDrawListMixinList_Axis3D> {};

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

