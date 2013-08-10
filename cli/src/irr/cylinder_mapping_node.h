// Ŭnicode please 
#pragma once

namespace irr {;
namespace scene {;
class CylinderMappingNode : public irr::scene::ISceneNode {
public:
	typedef irr::video::S3DVertex vertex_type;
	typedef std::vector<vertex_type> vertex_list_type;
	typedef std::vector<unsigned short> index_list_type;

public:
	CylinderMappingNode(irr::scene::ISceneNode *parent, 
		irr::scene::ISceneManager *smgr, 
		irr::s32 id,
		irr::video::ITexture *tex);
	virtual ~CylinderMappingNode();

	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const { return box_; }
	virtual irr::u32 getMaterialCount() const { return 1; }
	virtual irr::video::SMaterial &getMaterial(irr::u32 i) { return material_; }

public:
	float radius;

	void buildVertexList(irr::video::ITexture *tex, 
		float scale, 
		vertex_list_type *vertexList, 
		index_list_type *indexList);

protected:
	void renderBasic(const vertex_list_type &vertexList,
		const index_list_type &indexList);

protected:
	irr::core::aabbox3df box_;
	irr::video::SMaterial material_;

	std::vector<vertex_type> vertexList_;
	std::vector<unsigned short> indexList_;
};	
}	// namespace scene
}	// namespace irr