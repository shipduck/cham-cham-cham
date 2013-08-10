// Ŭnicode please 
#pragma once

namespace irr {;
namespace scene {;

/*
IBillboardTextSceneNode와 유사하지만 카메라 보는 방향으로 회전하지 않는다.
HMD용으로 구현
*/
class Text3dSceneNode : public ISceneNode {
public:
	Text3dSceneNode(ISceneNode* parent, 
		ISceneManager* mgr, 
		s32 id, 
		gui::IGUIFont* font,
		const wchar_t* text,
		const core::dimension2d<f32>& size = core::dimension2d<f32>(10.0f, 10.0f),
		const core::vector3df& position = core::vector3df(0,0,0));
	virtual ~Text3dSceneNode();

	virtual void OnRegisterSceneNode();
	virtual void render();

	virtual void setText(const wchar_t* text);
	virtual void setColor(video::SColor color);
	virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const { return box_; }
	virtual video::SMaterial& getMaterial(u32 i);
	virtual u32 getMaterialCount() const;
	virtual void OnAnimate(u32 timeMs);

	virtual void setSize(f32 height, f32 bottomEdgeWidth, f32 topEdgeWidth)
	{
		setSize(core::dimension2df(bottomEdgeWidth, height));
	}

	virtual void getSize(f32& height, f32& bottomEdgeWidth, f32& topEdgeWidth) const
	{
		height = size_.Height;
		bottomEdgeWidth = size_.Width;
		topEdgeWidth = size_.Width;
	}
	virtual void setSize(const core::dimension2d<f32>& size);


private:
	core::stringw text_;
	video::SColor color_;
	gui::IGUIFontBitmap* font_;

	core::dimension2d<f32> size_;
	core::aabbox3d<f32> box_;
	video::SMaterial material_;
	SMesh *mesh_;

	struct SSymbolInfo {
		u32 bufNo;
		f32 Width;
		f32 Kerning;
		u32 firstInd;
		u32 firstVert;
	};
	core::array < SSymbolInfo > symbol_;
};

}	// namespace scene
}	// namespace irr