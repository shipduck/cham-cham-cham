// Ŭnicode please 
#pragma once

class LineBatch {
public:
	typedef irr::video::S3DVertex vertex_type;
	typedef irr::video::SColor color_type;
	typedef irr::core::vector3df vector_3d_type;
	typedef irr::core::vector2di vector_2d_type;

public:
	LineBatch(float thickness=1.0f);

	void add(const vector_3d_type &p1, const vector_3d_type &p2, const color_type &color);
	void add(const vector_2d_type &p1, const vector_2d_type &p2, const color_type &color);

	void draw(irr::video::IVideoDriver *driver, bool depthEnable);
	void draw2D(irr::video::IVideoDriver *driver);
	void clear();

	template<typename VertexListType, typename IndexListType>
	void addIndexedVertices(const VertexListType &vertList, const IndexListType &idxList)
	{
		static_assert(std::is_same<vertex_type, VertexListType::value_type>::value == 1, "");
		static_assert(std::is_same<unsigned short, IndexListType::value_type>::value == 1, "");
		addIndexedVertices(vertList, idxList, &vertexList, &indexList);
	}

public:
	float thickness;
	std::vector<vertex_type> vertexList;
	std::vector<unsigned short> indexList;

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