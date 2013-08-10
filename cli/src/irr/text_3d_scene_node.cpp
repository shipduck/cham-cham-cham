// Ŭnicode please 
#include "stdafx.h"
#include "text_3d_scene_node.h"
#include "base/lib.h"

namespace irr {;
namespace scene {;

Text3dSceneNode::Text3dSceneNode(ISceneNode* parent, 
		ISceneManager* mgr, 
		s32 id, 
		gui::IGUIFont* font,
		const wchar_t* text,
		const core::dimension2d<f32>& size,
		const core::vector3df& position)
		: ISceneNode(parent, mgr, id, position),
		font_(nullptr),
		mesh_(nullptr)
{
	//일단 

	material_.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	material_.MaterialTypeParam = 1.f / 255.f;
	material_.BackfaceCulling = false;
	material_.Lighting = false;
	material_.ZBuffer = video::ECFN_LESSEQUAL;
	material_.ZWriteEnable = false;

	if (font) {
		// doesn't support other font types
		if (font->getType() == gui::EGFT_BITMAP) {
			font_ = (gui::IGUIFontBitmap*)font;
			font_->grab();

			// mesh with one buffer per texture
			mesh_ = new SMesh();
			for (u32 i=0; i < font_->getSpriteBank()->getTextureCount(); ++i) {
				SMeshBuffer *mb = new SMeshBuffer();
				mb->Material = material_;
				mb->Material.setTexture(0, font_->getSpriteBank()->getTexture(i));
				mesh_->addMeshBuffer(mb);
				mb->drop();
			}
		} else {
			Lib::printf("Sorry, CBillboardTextSceneNode does not support this font type");
		}
	}

	setText(text);
	setSize(size);

	setColor(irr::video::SColor(255, 255, 255, 255));
	//영역 잡기 귀찮으니까 일단 culling제거
	setAutomaticCulling(false);
	//setAutomaticCulling (scene::EAC_BOX);
}
Text3dSceneNode::~Text3dSceneNode()
{
	if (font_) {
		font_->drop();
	}

	if (mesh_) {
		mesh_->drop();
	}
}

//! pre render event
void Text3dSceneNode::OnAnimate(u32 timeMs)
{
	ISceneNode::OnAnimate(timeMs);

	if (!IsVisible || !font_ || !mesh_) {
		return;
	}

	// get text width
	f32 textLength = 0.f;
	for(u32 i=0; i!=symbol_.size(); ++i) {
		SSymbolInfo &info = symbol_[i];
		textLength += info.Kerning + info.Width;
	}
	if (textLength<0.0f) {
		textLength=1.0f;
	}

	core::vector3df pos;
	core::vector3df view(0, 0, -1);
	core::vector3df vertical(0, -1, 0);
	core::vector3df horizontal(1, 0, 0);

	horizontal *= 0.5f * size_.Width;
	vertical *= 0.5f * size_.Height;

	core::vector3df space = horizontal;

	// center text
	pos += space * (size_.Width * -0.5f);

	for (size_t i = 0; i!= symbol_.size(); ++i ) {
		SSymbolInfo &info = symbol_[i];
		f32 infw = info.Width / textLength;
		f32 infk = info.Kerning / textLength;
		f32 w = (size_.Width * infw * 0.5f);
		pos += space * w;

		SMeshBuffer* buf = (SMeshBuffer*)mesh_->getMeshBuffer(info.bufNo);

		buf->Vertices[info.firstVert+0].Normal = view;
		buf->Vertices[info.firstVert+1].Normal = view;
		buf->Vertices[info.firstVert+2].Normal = view;
		buf->Vertices[info.firstVert+3].Normal = view;

		buf->Vertices[info.firstVert+0].Pos = pos + (space * w) + vertical;
		buf->Vertices[info.firstVert+1].Pos = pos + (space * w) - vertical;
		buf->Vertices[info.firstVert+2].Pos = pos - (space * w) - vertical;
		buf->Vertices[info.firstVert+3].Pos = pos - (space * w) + vertical;

		pos += space * (size_.Width*infk + w);
	}

	// make bounding box

	for (size_t i=0; i< mesh_->getMeshBufferCount() ; ++i) {
		mesh_->getMeshBuffer(i)->recalculateBoundingBox();
	}
	mesh_->recalculateBoundingBox();

	box_ = mesh_->getBoundingBox();
	core::matrix4 mat( getAbsoluteTransformation(), core::matrix4::EM4CONST_INVERSE );
	mat.transformBoxEx(box_);
}

void Text3dSceneNode::setText(const wchar_t* text)
{
	if ( !mesh_ ) {
		return;
	}

	text_ = text;

	symbol_.clear();

	// clear mesh
	for (u32 j=0; j < mesh_->getMeshBufferCount(); ++j) {
		((SMeshBuffer*)mesh_->getMeshBuffer(j))->Indices.clear();
		((SMeshBuffer*)mesh_->getMeshBuffer(j))->Vertices.clear();
	}

	if (!font_) {
		return;
	}

	const core::array< core::rect<s32> > &sourceRects = font_->getSpriteBank()->getPositions();
	const core::array< gui::SGUISprite > &sprites = font_->getSpriteBank()->getSprites();

	f32 dim[2];
	f32 tex[4];

	for (u32 i = 0; i != text_.size (); ++i ) {
		SSymbolInfo info;

		u32 spriteno = font_->getSpriteNoFromChar( &text[i] );
		u32 rectno = sprites[spriteno].Frames[0].rectNumber;
		u32 texno = sprites[spriteno].Frames[0].textureNumber;

		dim[0] = core::reciprocal ( (f32) font_->getSpriteBank()->getTexture(texno)->getSize().Width );
		dim[1] = core::reciprocal ( (f32) font_->getSpriteBank()->getTexture(texno)->getSize().Height );

		const core::rect<s32>& s = sourceRects[rectno];

		// add space for letter to buffer
		SMeshBuffer* buf = (SMeshBuffer*)mesh_->getMeshBuffer(texno);
		u32 firstInd = buf->Indices.size();
		u32 firstVert = buf->Vertices.size();
		buf->Indices.set_used(firstInd + 6);
		buf->Vertices.set_used(firstVert + 4);

		tex[0] = (s.LowerRightCorner.X * dim[0]) + 0.5f*dim[0]; // half pixel
		tex[1] = (s.LowerRightCorner.Y * dim[1]) + 0.5f*dim[1];
		tex[2] = (s.UpperLeftCorner.Y  * dim[1]) - 0.5f*dim[1];
		tex[3] = (s.UpperLeftCorner.X  * dim[0]) - 0.5f*dim[0];

		buf->Vertices[firstVert+0].TCoords.set(tex[0], tex[1]);
		buf->Vertices[firstVert+1].TCoords.set(tex[0], tex[2]);
		buf->Vertices[firstVert+2].TCoords.set(tex[3], tex[2]);
		buf->Vertices[firstVert+3].TCoords.set(tex[3], tex[1]);

		buf->Vertices[firstVert+0].Color = color_;
		buf->Vertices[firstVert+3].Color = color_;
		buf->Vertices[firstVert+1].Color = color_;
		buf->Vertices[firstVert+2].Color = color_;

		buf->Indices[firstInd+0] = (u16)firstVert+0;
		buf->Indices[firstInd+1] = (u16)firstVert+2;
		buf->Indices[firstInd+2] = (u16)firstVert+1;
		buf->Indices[firstInd+3] = (u16)firstVert+0;
		buf->Indices[firstInd+4] = (u16)firstVert+3;
		buf->Indices[firstInd+5] = (u16)firstVert+2;

		wchar_t *tp = 0;
		if (i>0)
			tp = &text_[i-1];

		info.Width = (f32)s.getWidth();
		info.bufNo = texno;
		info.Kerning = (f32)font_->getKerningWidth(&text_[i], tp);
		info.firstInd = firstInd;
		info.firstVert = firstVert;

		symbol_.push_back(info);
	}
}
void Text3dSceneNode::OnRegisterSceneNode()
{
	if(IsVisible) {
		SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
	}
	ISceneNode::OnRegisterSceneNode();
}
void Text3dSceneNode::render()
{
	if ( !mesh_ ) {
		return;
	}

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	// draw
	core::matrix4 mat;
	//driver->setTransform(video::ETS_WORLD, mat);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);	

	for (u32 i = 0; i < mesh_->getMeshBufferCount(); ++i) {
		driver->setMaterial(mesh_->getMeshBuffer(i)->getMaterial());
		driver->drawMeshBuffer(mesh_->getMeshBuffer(i));
	}

	if ( DebugDataVisible & scene::EDS_BBOX ) {
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(box_, video::SColor(0,208,195,152));
	}
}
video::SMaterial& Text3dSceneNode::getMaterial(u32 i)
{
	if (mesh_ && mesh_->getMeshBufferCount() > i ) {
		return mesh_->getMeshBuffer(i)->getMaterial();
	} else {
		return material_;
	}
}
u32 Text3dSceneNode::getMaterialCount() const
{
	if (mesh_) { 
		return mesh_->getMeshBufferCount();
	} else {
		return 0;
	}
}
void Text3dSceneNode::setColor(video::SColor color)
{
	if ( !mesh_ ) {
		return;
	}

	for ( u32 i = 0; i != text_.size (); ++i ) {
		const SSymbolInfo &info = symbol_[i];
		SMeshBuffer* buf = (SMeshBuffer*)mesh_->getMeshBuffer(info.bufNo);
		buf->Vertices[info.firstVert+0].Color = color;
		buf->Vertices[info.firstVert+1].Color = color;
		buf->Vertices[info.firstVert+2].Color = color;
		buf->Vertices[info.firstVert+3].Color = color;
	}
}

//! sets the size of the billboard
void Text3dSceneNode::setSize(const core::dimension2d<f32>& size)
{
	size_ = size;

	if (size_.Width == 0.0f) {
		size_.Width = 1.0f;
	}

	if (size_.Height == 0.0f ) {
		size_.Height = 1.0f;
	}

	//f32 avg = (size.Width + size.Height)/6;
	//BBox.MinEdge.set(-avg,-avg,-avg);
	//BBox.MaxEdge.set(avg,avg,avg);
}


}	// namespace scene
}	// namespace irr