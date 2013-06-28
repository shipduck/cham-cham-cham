// Ŭnicode please 
#include "stdafx.h"
#include "line_batch_scene_node.h"

using namespace irr;
using namespace video;
using namespace core;

LineBatchSceneNode::LineBatchSceneNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, s32 id)
	: scene::ISceneNode(parent, smgr, id)
{
	Material.Wireframe = false;
	Material.Lighting = false;

	Box.reset(vector3df(0, 0, 0));

	VertexList.reserve(1 << 16);
	IndexList.reserve(1 << 16);
	Vertex2DList.reserve(1 << 16);
	Index2DList.reserve(1 << 16);
}

void LineBatchSceneNode::OnRegisterSceneNode()
{
	if(IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}
	ISceneNode::OnRegisterSceneNode();
}

void LineBatchSceneNode::render()
{
	IVideoDriver *driver = SceneManager->getVideoDriver();
	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(VertexList.data(), VertexList.size(), IndexList.data(), IndexList.size(), video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
	driver->draw2DVertexPrimitiveList(Vertex2DList.data(), Vertex2DList.size(), Index2DList.data(), Index2DList.size(), video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
}

void LineBatchSceneNode::addLine(const vector_type &p1, const vector_type &p2, const color_type &color)
{
	vector3df normal(1, 0, 0);
	vector2df texcoord(0, 0);
	auto v1 = vertex_type(p1, normal, color, texcoord);
	auto v2 = vertex_type(p2, normal, color, texcoord);

	unsigned short currVertexSize = VertexList.size();
	VertexList.push_back(v1);
	VertexList.push_back(v2);
	IndexList.push_back(currVertexSize);
	IndexList.push_back(currVertexSize + 1);
}

void LineBatchSceneNode::addLine(const vector_2d_type &p1, const vector_2d_type &p2, const color_type &color)
{
	vector3df normal(1, 0, 0);
	vector2df texcoord(0, 0);
	auto v1 = vertex_type(vector_type(p1.X, p1.Y, 0), normal, color, texcoord);
	auto v2 = vertex_type(vector_type(p2.X, p2.Y, 0), normal, color, texcoord);

	unsigned short currVertexSize = Vertex2DList.size();
	Vertex2DList.push_back(v1);
	Vertex2DList.push_back(v2);
	Index2DList.push_back(currVertexSize);
	Index2DList.push_back(currVertexSize + 1);
}

void LineBatchSceneNode::clear()
{
	VertexList.clear();
	IndexList.clear();
	Vertex2DList.clear();
	Index2DList.clear();
}
