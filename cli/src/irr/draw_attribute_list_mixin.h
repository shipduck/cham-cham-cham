// Ŭnicode please 
#pragma once

#include <string>
#include <vector>

template<typename T>
void swapVectorElement(T &vec, int a, int b)
{
	SR_ASSERT(a >= 0 && a < static_cast<int>(vec.size()));
	SR_ASSERT(b >= 0 && b < static_cast<int>(vec.size()));
	T::value_type tmp = vec[a];
	vec[a] = vec[b];
	vec[b] = tmp;
}

class DrawAttributeListMixin_Color {
public:
	typedef DrawAttributeListMixin_Color mixin_type;
public:
	std::vector<irr::video::SColor> colorList;	
	void clear() { colorList.clear(); }
	void pop_back() { colorList.pop_back(); }
	size_t size() const { return colorList.size(); }
	void swap(int a, int b) { swapVectorElement(colorList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->colorList.push_back(colorList[idx]);
	}
};

class DrawAttributeListMixin_Node {
public:
	typedef DrawAttributeListMixin_Node mixin_type;
public:
	std::vector<irr::scene::ISceneNode*> nodeList;
	void clear();
	void pop_back();
	size_t size() const { return nodeList.size(); }
	void swap(int a, int b) { swapVectorElement(nodeList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->nodeList.push_back(nodeList[idx]);
	}
};

class DrawAttributeListMixin_3D {
public:
	typedef DrawAttributeListMixin_3D mixin_type;
public:
	std::vector<bool> depthEnableList;
	void clear() { depthEnableList.clear(); }
	void pop_back() { depthEnableList.pop_back(); }
	size_t size() const { return depthEnableList.size(); }
	void swap(int a, int b) { swapVectorElement(depthEnableList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->depthEnableList.push_back(depthEnableList[idx]);
	}
};

template<typename T>
class DrawAttributeListMixin_Scale {
public:
	typedef DrawAttributeListMixin_Scale<T> mixin_type;
public:
	std::vector<T> scaleList;
	void clear() { scaleList.clear(); }
	void pop_back() { scaleList.pop_back(); }
	size_t size() const { return scaleList.size(); }
	void swap(int a, int b) { swapVectorElement(scaleList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->scaleList.push_back(scaleList[idx]);
	}
};

class DrawAttributeListMixin_Transform {
public:
	typedef DrawAttributeListMixin_Transform mixin_type;
public:
	std::vector<irr::core::matrix4> xfList;
	void clear() { xfList.clear(); }
	void pop_back() { xfList.pop_back(); }
	size_t size() const { return xfList.size(); }
	void swap(int a, int b) { swapVectorElement(xfList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->xfList.push_back(xfList[idx]);
	}
};

template<typename T>
class DrawAttributeListMixin_Line {
public:
	typedef DrawAttributeListMixin_Line mixin_type;
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
	void swap(int a, int b)
	{
		swapVectorElement(p1List, a, b);
		swapVectorElement(p2List, a, b);
	}
	void copy_elem(mixin_type *target, int idx)
	{
		target->p1List.push_back(p1List[idx]);
		target->p2List.push_back(p2List[idx]);
	}
};

template<typename T>
class DrawAttributeListMixin_Pos {
public:
	typedef DrawAttributeListMixin_Pos<T> mixin_type;
public:
	std::vector<T> posList;
	void clear() { posList.clear(); }
	void pop_back() { posList.pop_back(); }
	size_t size() const { return posList.size(); }
	void swap(int a, int b) { swapVectorElement(posList, a, b); }
	void copy_elem(mixin_type *target, int idx)
	{
		target->posList.push_back(posList[idx]);
	}
};

class DrawAttributeListMixin_String {
public:
	typedef DrawAttributeListMixin_String mixin_type;
public:
	typedef std::wstring string_type;
	typedef std::wstring::value_type value_type;

	std::vector<string_type> msgList;

	void clear() { msgList.clear(); }
	void pop_back() { msgList.pop_back(); }
	size_t size() const { return msgList.size(); }
	void swap(int a, int b) { swapVectorElement(msgList, a, b); }

	void copy_elem(mixin_type *target, int idx)
	{
		target->msgList.push_back(msgList[idx]);
	}
};