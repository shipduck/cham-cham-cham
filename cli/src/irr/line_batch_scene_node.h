// Ŭnicode please 
#pragma once

#include "line_batch.h"

class LineBatchSceneNode : public irr::scene::ISceneNode {
public:
	typedef irr::video::SColor color_type;
	typedef irr::video::S3DVertex vertex_type;
	typedef irr::core::vector3df vector_3d_type;
	typedef irr::core::vector2di vector_2d_type;

public:
	LineBatchSceneNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, irr::s32 id);

	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const { return box_; }
	virtual irr::u32 getMaterialCount() const { return 1; }
	virtual irr::video::SMaterial &getMaterial(irr::u32 i) { return material_; }

	void addLine(const vector_3d_type &p1, const vector_3d_type &p2, const color_type &color);
	void addLine(const vector_2d_type &p1, const vector_2d_type &p2, const color_type &color);

	void setThickness(float val);

	template<typename VertexListType, typename IndexListType>
	void addIndexedVertices(const VertexListType &vertexList, const IndexListType &indexList)
	{
		batch3D_.addIndexedVertices(vertexList, indexList);
	}
	template<typename VertexListType, typename IndexListType>
	void addIndexedVertices2D(const VertexListType &vertexList, const IndexListType &indexList)
	{
		batch2D_.addIndexedVertices(vertexList, indexList);
	}
	
	void clear();

private:
	irr::core::aabbox3d<irr::f32> box_;
	irr::video::SMaterial material_;
	LineBatch batch3D_;
	LineBatch batch2D_;
};

