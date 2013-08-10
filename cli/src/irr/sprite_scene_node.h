// Ŭnicode please 
#pragma once

namespace irr {;
namespace scene {;
class SpriteSceneNode : public irr::scene::ISceneNode {
public:
	//! constructor
	SpriteSceneNode(irr::scene::ISceneNode* parent, 
		irr::scene::ISceneManager* mgr, 
		s32 id,
		irr::video::ITexture *tex);
	virtual ~SpriteSceneNode();

	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const { return box_; }
	virtual irr::u32 getMaterialCount() const { return 1; }
	virtual irr::video::SMaterial &getMaterial(irr::u32 i) { return material_; }

	void setSize(const core::dimension2d<f32>& size) { size_ = size; }
	const core::dimension2d<f32> &getSize() const { return size_; }

private:
	//! Size.Width is the bottom edge width
	core::dimension2d<f32> size_;
	video::SMaterial material_;
	core::aabbox3d<f32> box_;
	std::array<video::S3DVertex, 4> vertices_;
	std::array<u16, 6> indices_;
};
}	// namespace scene
}	// namespace irr