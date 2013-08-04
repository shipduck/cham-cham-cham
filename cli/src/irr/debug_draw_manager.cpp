// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_manager.h"

using namespace Loki;
using namespace irr;
using namespace video;
using namespace scene;
using namespace core;
using namespace gui;

//singleton
DebugDrawManager debugDrawMgrLocal;
DebugDrawManager *g_debugDrawMgr = &debugDrawMgrLocal;

template<typename TList> struct DebugDrawListFunctor;
template<>
struct DebugDrawListFunctor<NullType> {
	static int size(const DebugDrawManager &mgr) { return 0; }
	static void clear(DebugDrawManager &mgr) { return; }
	static void update(int ms, DebugDrawManager &mgr) { return; }
};
template<typename T, typename U>
struct DebugDrawListFunctor< Typelist<T, U> > {
	static int size(const DebugDrawManager &mgr)
	{
		int sum = 0;
		sum += Field<T>(mgr).immediateDrawList.size();
		sum += Field<T>(mgr).durationDrawList.size();
		sum += DebugDrawListFunctor<U>::size(mgr);
		return sum;
	}
	static void clear(DebugDrawManager &mgr)
	{
		Field<T>(mgr).durationList.clear();
		Field<T>(mgr).durationDrawList.clear();
		Field<T>(mgr).immediateDrawList.clear();
		DebugDrawListFunctor<U>::clear(mgr);
	}
	static void update(int ms, DebugDrawManager &mgr)
	{
		Field<T>(mgr).immediateDrawList.clear();
		
		auto &durationList = Field<T>(mgr).durationList;
		auto &durationDrawList = Field<T>(mgr).durationDrawList;
		SR_ASSERT(durationList.size() == durationDrawList.size());

		//경과 시간 처리
		for(size_t i = 0 ; i < durationList.size() ; ++i) {
			durationList[i] -= ms;
		}
		while(durationList.empty() == false) {
			int duration = durationList.back();
			if(duration > 0) {
				break;
			}
			durationList.pop_back();
		}

		typedef std::tuple<int, int> DurationIndexTuple;
		std::vector<DurationIndexTuple> durationIndexList(durationList.size());
		for(size_t i = 0 ; i < durationList.size() ; ++i) {
			DurationIndexTuple &dataTuple = durationIndexList[i];
			std::get<0>(dataTuple) = durationList[i];
			std::get<1>(dataTuple) = i;
		}
		//durationDrawList-durationList 시간 내림차순으로 정렬
		std::sort(durationIndexList.begin(),
			durationIndexList.end(),
			[](const DurationIndexTuple &a, const DurationIndexTuple &b) {
				return std::get<0>(a) > std::get<0>(b);
		});
		std::sort(durationList.begin(), 
			durationList.end(),
			[](int a, int b) { return a > b; });

		//index 순서대로 복사, 경과시간 고려해서 지나친항목은 무시
		typedef typename std::remove_reference<decltype(durationDrawList)>::type DurationDrawList;
		DurationDrawList nextDurationDrawList;
		for(const auto &dataTuple : durationIndexList) {
			int idx = std::get<1>(dataTuple);
			int duration = std::get<0>(dataTuple);
			if(duration > 0) {
				durationDrawList.copy_elem(&nextDurationDrawList, idx);
			}
		}
		durationDrawList = std::move(nextDurationDrawList);

		SR_ASSERT(durationList.size() == durationDrawList.size());
		DebugDrawListFunctor<U>::update(ms, mgr);
	}
};

size_t DebugDrawManager::size() const
{
	int sum = DebugDrawListFunctor<DrawAttributeElemList>::size(*this);	
	return sum;
}

DebugDrawManager::DebugDrawManager()
{
}
DebugDrawManager::~DebugDrawManager()
{
	clear();
}

void DebugDrawManager::clear() 
{
	DebugDrawListFunctor<DrawAttributeElemList>::clear(*this);
}

void DebugDrawManager::updateAll(int ms)
{
	DebugDrawListFunctor<DrawAttributeElemList>::update(ms, *this);
}

void DebugDrawManager::addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Line3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.p1List.push_back(p1);
	drawList.p2List.push_back(p2);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(lineWidth);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCross(const irr::core::vector3df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Cross3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(size);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addSphere(const irr::core::vector3df &pos, 
		float radius,
		const irr::video::SColor &color,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Sphere3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(radius);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_Axis3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.xfList.push_back(xf);
	drawList.scaleList.push_back(size);
	drawList.depthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addString(const irr::core::vector3df &pos, 
		const DrawAttributeListMixin_String::string_type &msg,
		const irr::video::SColor &color,
		float scale,
		int duration,
		bool depthEnable)
{
	typedef DrawAttributeList_String3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(scale);
	drawList.textList.push_back(msg);
	drawList.depthEnableList.push_back(depthEnable);
	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration)
{
	typedef DrawAttributeList_Line2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.p1List.push_back(p1);
	drawList.p2List.push_back(p2);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(lineWidth);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCross(const irr::core::vector2di &pos, 
		const irr::video::SColor &color,
		float size,
		int duration)
{
	typedef DrawAttributeList_Cross2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(size);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addString(const irr::core::vector2di &pos, 
								   const DrawAttributeListMixin_String::string_type &msg,
								   const irr::video::SColor &color,
								   float scale,
								   int duration)
{
	typedef DrawAttributeList_String2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(scale);
	drawList.textList.push_back(msg);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration)
{
	typedef DrawAttributeList_Circle2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.posList.push_back(pos);
	drawList.colorList.push_back(color);
	drawList.scaleList.push_back(radius);

	Field<ListType>(*this).runValidateOnce(duration);
}
