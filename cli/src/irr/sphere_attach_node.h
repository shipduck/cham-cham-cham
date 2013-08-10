// Ŭnicode please 
#pragma once

class SphereAttachNode : public irr::scene::ISceneNode {
public:
	typedef irr::video::S3DVertex vertex_type;
	typedef std::vector<vertex_type> vertex_list_type;
	typedef std::vector<unsigned short> index_list_type;

public:
	SphereAttachNode(irr::scene::ISceneNode *parent, 
		irr::scene::ISceneManager *smgr, 
		irr::s32 id,
		irr::video::ITexture *tex);
	virtual ~SphereAttachNode();

	virtual void OnRegisterSceneNode();
	virtual void render();

	virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const { return box_; }
	virtual irr::u32 getMaterialCount() const { return 1; }
	virtual irr::video::SMaterial &getMaterial(irr::u32 i) { return material_; }

public:
	float scale;
	float radius;

protected:
	irr::core::aabbox3df box_;
	irr::video::SMaterial material_;

	irr::video::ITexture *tex_;
};
