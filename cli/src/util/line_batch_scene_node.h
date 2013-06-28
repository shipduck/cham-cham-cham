// Ŭnicode please 
#pragma once

class LineBatchSceneNode : public irr::scene::ISceneNode {
public:
	typedef irr::core::vector3df vector_type;
	typedef irr::video::SColor color_type;
	typedef irr::video::S3DVertex vertex_type;
	typedef irr::core::vector2di vector_2d_type;
public:
	LineBatchSceneNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, irr::s32 id);

	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const { return Box; }
	virtual irr::u32 getMaterialCount() const { return 1; }
	virtual irr::video::SMaterial &getMaterial(irr::u32 i) { return Material; }

	void addLine(const vector_type &p1, const vector_type &p2, const color_type &color);
	void addLine(const vector_2d_type &p1, const vector_2d_type &p2, const color_type &color);

	void setThickness(float val) { Material.Thickness = val; }
	float getThickness() { return Material.Thickness; }

	template<typename VertexListType, typename IndexListType>
	void addIndexedVertices(const VertexListType &vertexList, const IndexListType &indexList)
	{
		static_assert(std::is_same<vertex_type, VertexListType::value_type>::value == 1, "");
		static_assert(std::is_same<unsigned short, IndexListType::value_type>::value == 1, "");
		addIndexedVertices(vertexList, indexList, &VertexList, &IndexList);
	}
	template<typename VertexListType, typename IndexListType>
	void addIndexedVertices2D(const VertexListType &vertexList, const IndexListType &indexList)
	{
		static_assert(std::is_same<vertex_type, VertexListType::value_type>::value == 1, "");
		static_assert(std::is_same<unsigned short, IndexListType::value_type>::value == 1, "");
		addIndexedVertices(vertexList, indexList, &Vertex2DList, &Index2DList);
	}
	
	void clear();

private:
	irr::core::aabbox3d<irr::f32> Box;
	irr::video::SMaterial Material;

	std::vector<vertex_type> VertexList;
	std::vector<unsigned short> IndexList;

	std::vector<vertex_type> Vertex2DList;
	std::vector<unsigned short> Index2DList;

private:
	template<typename InputVertexListType, typename InputIndexListType>
	void addIndexedVertices(const InputVertexListType &vertexList, 
		const InputIndexListType &indexList,
		std::vector<vertex_type> *targetVertexList,
		std::vector<unsigned short> *targetIndexList)
	{
		static_assert(std::is_same<vertex_type, InputVertexListType::value_type>::value == 1, "");
		static_assert(std::is_same<unsigned short, InputIndexListType::value_type>::value == 1, "");

		int currVertexSize = targetVertexList->size();
		std::copy(vertexList.begin(), vertexList.end(), std::back_inserter(*targetVertexList));
		for(auto idx : indexList) {
			auto nextIdx = idx + currVertexSize;
			targetIndexList->push_back(nextIdx);
		}
	}
};

