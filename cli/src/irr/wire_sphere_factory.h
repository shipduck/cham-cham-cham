// Ŭnicode please 
#pragma once

template<int Slices, int Stacks>
class WireSphereFactory {
public:
	enum {
		kVertexCount = ((Stacks - 1) * Slices) + (Slices * (Stacks + 1)),
		kIndexCount = ((Stacks - 1) * Slices * 2) + (Slices * (Stacks) * 2)
	};

	typedef irr::core::vector3df vector_type;
	typedef irr::video::S3DVertex vertex_type;
	typedef unsigned short index_type;
	typedef std::array<vertex_type, kVertexCount> vertex_list_type;
	typedef std::array<index_type, kIndexCount> index_list_type;


public:
	template<typename VertexListType, typename IndexListType>
	static void createSimpleMesh(VertexListType &vertexList, IndexListType &indexList)
	{
		static_assert(std::is_same<vertex_type, typename VertexListType::value_type>::value == 1, "");
		static_assert(std::is_same<index_type, typename IndexListType::value_type>::value == 1, "");
		SR_ASSERT(vertexList.size() == kVertexCount);
		SR_ASSERT(indexList.size() == kIndexCount);

		const float drho = irr::core::PI / Stacks;
		const float dtheta = 2.0f * irr::core::PI / Slices;

		//한방에 그릴수 잇도록하자.
		//vertex list + index로 구성을 변경한다는 소리
		//구성 방법은 gl_lines

		int currIndexListSize = 0;
		int currVertexListSize = 0;

		// draw stack lines
		for (int i = 1 ; i < Stacks ; i++) {
			// stack line at i==stacks-1 was missing here
			float rho = i * drho;

			std::array<vertex_type, Slices> tmp_vert_list;
			for (int j = 0; j < Slices; j++) {
				float theta = j * dtheta;
				float x = cos(theta) * sin(rho);
				float y = sin(theta) * sin(rho);
				float z = cos(rho);

				vertex_type &vert = tmp_vert_list[j];
				vert.Pos.X = x;
				vert.Pos.Y = y;
				vert.Pos.Z = z;
			}
			auto vertexListIt = vertexList.begin();
			std::advance(vertexListIt, currVertexListSize);
			std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), vertexListIt);

			for(size_t i = 0 ; i < tmp_vert_list.size() ; i++) {
				unsigned short idx1 = currVertexListSize + i;
				unsigned short idx2 = currVertexListSize + ((i + 1) % (int)tmp_vert_list.size());
				indexList[currIndexListSize + i*2 + 0] = idx1;
				indexList[currIndexListSize + i*2 + 1] = idx2;
			}
			currVertexListSize += tmp_vert_list.size();
			currIndexListSize += tmp_vert_list.size() * 2;
		}
		// draw slice lines
		for (int j = 0; j < Slices; j++) {
			float theta = j * dtheta;

			std::array<vertex_type, Stacks + 1> tmp_vert_list;
			for (int i = 0; i <= Stacks ; i++) {
				float rho = i * drho;
				float x = cos(theta) * sin(rho);
				float y = sin(theta) * sin(rho);
				float z = cos(rho);

				vertex_type &vert = tmp_vert_list[i];
				vert.Pos.X = x;
				vert.Pos.Y = y;
				vert.Pos.Z = z;
			}
			auto vertexListIt = vertexList.begin();
			std::advance(vertexListIt, currVertexListSize);
			std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), vertexListIt);

			for(size_t i = 0 ; i < tmp_vert_list.size() - 1 ; i++) {
				unsigned short idx1 = currVertexListSize + i;
				unsigned short idx2 = currVertexListSize + ((i + 1) % (int)tmp_vert_list.size());
				indexList[currIndexListSize + i*2 + 0] = idx1;
				indexList[currIndexListSize + i*2 + 1] = idx2;
			}
			currVertexListSize += tmp_vert_list.size();
			currIndexListSize +=  (tmp_vert_list.size() - 1) * 2;
		}
	}
};
