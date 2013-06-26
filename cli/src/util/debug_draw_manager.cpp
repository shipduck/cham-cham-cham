// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_manager.h"

using namespace Loki;
using namespace irr;
using namespace video;
using namespace scene;
using namespace core;
using namespace gui;

//singleton
DebugDrawManager debugDrawMgrLocal;
DebugDrawManager *gDebugDrawMgr = &debugDrawMgrLocal;

irr::gui::IGUIFont *gNormalFont12 = nullptr;
irr::gui::IGUIFont *gNormalFont14 = nullptr;

class WireSphereFactory {
public:
	typedef irr::video::SColor color_type;
	typedef irr::core::vector3df vector_type;
	typedef irr::video::S3DVertex vertex_type;
	typedef unsigned short index_type;
	typedef std::vector<vertex_type> vertex_list_type;
	typedef std::vector<index_type> index_list_type;

public:
	WireSphereFactory(float radius, int slices, int stacks, const color_type &color)
		: Radius(radius), Slices(slices), Stacks(stacks), Color(color) {}
	~WireSphereFactory() {}

	void createSimpleMesh(vertex_list_type &vertexList, index_list_type &indexList);

private:
	float Radius;
	int Slices;
	int Stacks;
	color_type Color;
};

void WireSphereFactory::createSimpleMesh(vertex_list_type &vertexList, index_list_type &indexList)
{
	float nsign = 1.0f;
	float drho = PI / Stacks;
	float dtheta = 2.0f * PI / Slices;

	//한방에 그릴수 잇도록하자.
	//vertex list + index로 구성을 변경한다는 소리
	//구성 방법은 gl_lines

	// draw stack lines
	for (int i = 1 ; i < Stacks ; i++) { // stack line at i==stacks-1 was missing here
		float rho = i * drho;

		vertex_list_type tmp_vert_list;
		for (int j = 0; j < Slices; j++) {
			float theta = j * dtheta;
			float x = cos(theta) * sin(rho);
			float y = sin(theta) * sin(rho);
			float z = cos(rho);

			vertex_type vert;
			//vert.n[0] = x * nsign;
			//vert.n[1] = y * nsign;
			//vert.n[2] = z * nsign;

			vert.Pos.X = x * Radius;
			vert.Pos.Y = y * Radius;
			vert.Pos.Z = z * Radius;
			vert.Color = this->Color;

			tmp_vert_list.push_back(vert);
		}
		int base_vert_list_size = vertexList.size();
		//copy vertex
		std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), std::back_inserter(vertexList));

		for(int i = 0 ; i < (int)tmp_vert_list.size() ; i++) {
			unsigned short idx1 = base_vert_list_size + i;
			unsigned short idx2 = base_vert_list_size + ((i + 1) % (int)tmp_vert_list.size());
			indexList.push_back(idx1);
			indexList.push_back(idx2);
		}
	}
	// draw slice lines
	for (int j = 0; j < Slices; j++) {
		float theta = j * dtheta;

		vertex_list_type tmp_vert_list;
		for (int i = 0; i <= Stacks ; i++) {
			float rho = i * drho;
			float x = cos(theta) * sin(rho);
			float y = sin(theta) * sin(rho);
			float z = cos(rho);

			vertex_type vert;
			//vert.n[0] = x * nsign;
			//vert.n[1] = y * nsign;
			//vert.n[2] = z * nsign;

			vert.Pos.X = x * Radius;
			vert.Pos.Y = y * Radius;
			vert.Pos.Z = z * Radius;
			vert.Color = this->Color;

			tmp_vert_list.push_back(vert);
		}
		int base_vert_list_size = vertexList.size();
		std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), std::back_inserter(vertexList));

		for(int i = 0 ; i < (int)tmp_vert_list.size() - 1 ; i++) {
			unsigned short idx1 = base_vert_list_size + i;
			unsigned short idx2 = base_vert_list_size + ((i + 1) % (int)tmp_vert_list.size());
			indexList.push_back(idx1);
			indexList.push_back(idx2);
		}
	}
}



DebugDrawListMixin_Node::~DebugDrawListMixin_Node()
{
	clear();
}
void DebugDrawListMixin_Node::pop_back() 
{
	auto node = NodeList.back();
	if(node) {
		node->remove();
		node->drop();
	}
	NodeList.pop_back();
}
void DebugDrawListMixin_Node::clear() 
{
	auto it = NodeList.begin();
	auto endit = NodeList.end();
	for( ; it != endit ; ++it) {
		auto node = *it;
		if(node) {
			node->remove();
			node->drop();
		}
	}
	NodeList.clear(); 
}

void DebugDrawManager::setUp(irr::IrrlichtDevice *dev)
{
	this->Device = dev;

	ISceneManager* smgr = dev->getSceneManager();
	batchSceneNode = new DebugDrawSceneNode(smgr->getRootSceneNode(), smgr, 0);
}
void DebugDrawManager::shutDown()
{
	clear();
	if(batchSceneNode) {
		batchSceneNode->remove();
		batchSceneNode->drop();
		batchSceneNode = nullptr;
	}
}


template<typename TList> struct DebugDrawListFunctor;
template<>
struct DebugDrawListFunctor<NullType> {
	static int size(const DebugDrawManager &mgr) { return 0; }
	static void draw(DebugDrawManager &mgr) { return; }
	static void clear(DebugDrawManager &mgr) { return; }
	static void update(int ms, DebugDrawManager &mgr) { return; }
};
template<typename T, typename U>
struct DebugDrawListFunctor< Typelist<T, U> > {
	static int size(const DebugDrawManager &mgr)
	{
		int sum = 0;
		sum += Field<T>(mgr).ImmediateDrawList.size();
		sum += Field<T>(mgr).DurationDrawList.size();
		sum += DebugDrawListFunctor<U>::size(mgr);
		return sum;
	}
	static void draw(DebugDrawManager &mgr)
	{
		const auto &immediateList = Field<T>(mgr).ImmediateDrawList;
		const auto &durationList = Field<T>(mgr).DurationDrawList;
		if(immediateList.size() > 0) {
			mgr.drawList(immediateList);
		}
		if(durationList.size() > 0) {
			mgr.drawList(durationList);
		}
		DebugDrawListFunctor<U>::draw(mgr);
	}
	static void clear(DebugDrawManager &mgr)
	{
		Field<T>(mgr).DurationList.clear();
		Field<T>(mgr).DurationDrawList.clear();
		Field<T>(mgr).ImmediateDrawList.clear();
		DebugDrawListFunctor<U>::clear(mgr);
	}
	static void update(int ms, DebugDrawManager &mgr)
	{
		Field<T>(mgr).ImmediateDrawList.clear();
		
		auto &durationList = Field<T>(mgr).DurationList;
		auto &durationDrawList = Field<T>(mgr).DurationDrawList;

		//TODO DurationDrawList-DurationList 시간 내림차순으로 정렬
		SR_ASSERT(durationList.size() == 0 && "duration based is not implemented yet");

		//시간지난거 삭제
		for(size_t i = 0 ; i < durationList.size() ; ++i) {
			durationList[i] -= ms;
		}
		while(durationList.empty() == false) {
			int duration = durationList.back();
			if(duration > 0) {
				break;
			}

			durationList.pop_back();
			durationDrawList.pop_back();
		}

		DebugDrawListFunctor<U>::update(ms, mgr);
	}
};

int DebugDrawManager::size() const
{
	int sum = DebugDrawListFunctor<DebugDrawCmdTypeList>::size(*this);	
	return sum;
}


void DebugDrawManager::drawAll()
{
	if(batchSceneNode) {
		batchSceneNode->clear();
	}
	DebugDrawListFunctor<DebugDrawCmdTypeList>::draw(*this);
	if(batchSceneNode) {
		batchSceneNode->render();
	}
}

void DebugDrawManager::clear() 
{
	DebugDrawListFunctor<DebugDrawCmdTypeList>::clear(*this);
}

void DebugDrawManager::updateAll(int ms)
{
	DebugDrawListFunctor<DebugDrawCmdTypeList>::update(ms, *this);
}

void DebugDrawManager::addLine(const irr::core::vector3df &p1, const irr::core::vector3df &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_Line3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.P1List.push_back(p1);
	drawList.P2List.push_back(p2);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(lineWidth);
	drawList.DepthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCross(const irr::core::vector3df &pos, 
		const irr::video::SColor &color,
		float size,
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_Cross3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(size);
	drawList.DepthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addSphere(const irr::core::vector3df &pos, 
		float radius,
		const irr::video::SColor &color,
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_Sphere3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(radius);
	drawList.DepthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addAxis(const irr::core::matrix4 &xf,
		float size, 
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_Axis3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.XfList.push_back(xf);
	drawList.ScaleList.push_back(size);
	drawList.DepthEnableList.push_back(depthEnable);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addString(const irr::core::vector3df &pos, 
		const DebugDrawListMixin_String::string_type &msg,
		const irr::video::SColor &color,
		float scale,
		int duration,
		bool depthEnable)
{
	typedef DebugDrawList_String3D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(scale);
	drawList.MsgList.push_back(msg);
	drawList.DepthEnableList.push_back(depthEnable);

	//create scene node
	ISceneManager* smgr = Device->getSceneManager();
	ITextSceneNode *node = smgr->addTextSceneNode(gNormalFont14, msg.data(), color);
	if(node) {
		SMaterial material;
		material.Wireframe = true;
		material.Lighting = false;
		material.BackfaceCulling = false;
		material.FrontfaceCulling = false;
		node->getMaterial(0) = material;
	}
	node->setScale(vector3df(scale, scale, scale));
	node->setPosition(pos);
	node->grab();
	drawList.NodeList.push_back(node);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addLine(const irr::core::vector2di &p1, const irr::core::vector2di &p2,
		const irr::video::SColor &color,
		float lineWidth,
		int duration)
{
	typedef DebugDrawList_Line2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.P1List.push_back(p1);
	drawList.P2List.push_back(p2);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(lineWidth);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCross(const irr::core::vector2di &pos, 
		const irr::video::SColor &color,
		float size,
		int duration)
{
	typedef DebugDrawList_Cross2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);

	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(size);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addString(const irr::core::vector2di &pos, 
								   const DebugDrawListMixin_String::string_type &msg,
								   const irr::video::SColor &color,
								   float scale,
								   int duration)
{
	typedef DebugDrawList_String2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(scale);
	drawList.MsgList.push_back(msg);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::addCircle(const irr::core::vector2di &pos, float radius,
		const irr::video::SColor &color,
		int duration)
{
	typedef DebugDrawList_Circle2D ListType;
	ListType &drawList = Field<ListType>(*this).getListAndPushDuration(duration);
	
	drawList.PosList.push_back(pos);
	drawList.ColorList.push_back(color);
	drawList.ScaleList.push_back(radius);

	Field<ListType>(*this).runValidateOnce(duration);
}

void DebugDrawManager::drawList(const DebugDrawList_Line2D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.ScaleList[i];
		if(scale != 1.0f) {
			continue;
		}
		batchSceneNode->addLine(cmd.P1List[i], cmd.P2List[i], cmd.ColorList[i]);
	}

	auto driver = Device->getVideoDriver();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.ScaleList[i];
		if(scale == 1.0f) {
			continue;
		}
		video::SMaterial m; 
		m.Lighting = false;
		m.Thickness = scale;
		driver->setMaterial(m);
		driver->draw2DLine(cmd.P1List[i], cmd.P2List[i], cmd.ColorList[i]);
	}
}

void DebugDrawManager::drawList(const DebugDrawList_Cross2D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		auto pos = cmd.PosList[i];
		auto size = static_cast<int>(cmd.ScaleList[i]);
		auto color = cmd.ColorList[i];

		vector2di top = pos + vector2di(0, size);
		vector2di bottom = pos - vector2di(0, size);
		vector2di right = pos + vector2di(size, 0);
		vector2di left = pos - vector2di(size, 0);

		batchSceneNode->addLine(top, bottom, color);
		batchSceneNode->addLine(left, right, color);
	}
}
void DebugDrawManager::drawList(const DebugDrawList_String2D &cmd)
{
	if(!gNormalFont14) {
		return;
	}

	auto driver = Device->getVideoDriver();
	const irr::core::dimension2du& screenSize = driver->getScreenSize();
	int w = screenSize.Width;
	int h = screenSize.Height;

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		auto pos = cmd.PosList[i];
		auto color = cmd.ColorList[i];
		auto msg = cmd.MsgList[i];
		int x = pos.X;
		int y = pos.Y;
		gNormalFont14->draw(msg.data(), rect<s32>(x, y, w, h), color);
	}
}

void DebugDrawManager::drawList(const DebugDrawList_Circle2D &cmd)
{
	SR_ASSERT(false && "NotImplemented");
}

void DebugDrawManager::drawList(const DebugDrawList_Line3D &cmd)
{
	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.ScaleList[i];
		if(scale != 1.0f) {
			continue;
		}
		batchSceneNode->addLine(cmd.P1List[i], cmd.P2List[i], cmd.ColorList[i]);
	}

	IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, core::matrix4());
	for(int i = 0 ; i < loopCount ; ++i) {
		float scale = cmd.ScaleList[i];
		if(scale == 1.0f) {
			continue;
		}

		video::SMaterial m; 
		m.Lighting = false;
		m.Thickness = cmd.ScaleList[i];
		driver->setMaterial(m);
		driver->draw3DLine(cmd.P1List[i], cmd.P2List[i], cmd.ColorList[i]);
	}
}

void DebugDrawManager::drawList(const DebugDrawList_Cross3D &cmd)
{
	//항상 동일한 크기로 보이게 적절히 렌더링하기
}

void DebugDrawManager::drawList(const DebugDrawList_Sphere3D &cmd)
{
	static WireSphereFactory::vertex_list_type baseVertexList;
	static WireSphereFactory::index_list_type baseIndexList;
	if(baseVertexList.empty()) {
		SColor white(255, 255, 255, 255);
		WireSphereFactory factory(1, 8, 8, white);
		factory.createSimpleMesh(baseVertexList, baseIndexList);
	}

	WireSphereFactory::vertex_list_type vertexList(baseVertexList.size());

	for(size_t i = 0 ; i < cmd.size() ; i++) {
		auto color = cmd.ColorList[i];
		auto pos = cmd.PosList[i];
		auto scale = cmd.ScaleList[i];

		std::copy(baseVertexList.begin(), baseVertexList.end(), vertexList.begin());
		for(int i = 0 ; i < vertexList.size() ; ++i) {
			auto &vert = vertexList[i];
			vert.Color = color;
			vert.Pos *= scale;
			vert.Pos += pos;
		}

		batchSceneNode->addIndexedVertices(vertexList, baseIndexList);
	}
}
void DebugDrawManager::drawList(const DebugDrawList_String3D &cmd)
{
	//use scene node based
}

void DebugDrawManager::drawList(const DebugDrawList_Axis3D &cmd)
{
	SColor red(255, 255, 0, 0);
	SColor green(255, 0, 255, 0);
	SColor blue(255, 0, 0, 255);

	int loopCount = cmd.size();
	for(int i = 0 ; i < loopCount ; ++i) {
		auto size = cmd.ScaleList[i];
		auto xf = cmd.XfList[i];

		vector3df zero(0, 0, 0);
		vector3df x(size, 0, 0);
		vector3df y(0, size, 0);
		vector3df z(0, 0, size);

		xf.transformVect(zero);
		xf.transformVect(x);
		xf.transformVect(y);
		xf.transformVect(z);

		batchSceneNode->addLine(zero, x, red);
		batchSceneNode->addLine(zero, y, green);
		batchSceneNode->addLine(zero, z, blue);
	}
}


DebugDrawSceneNode::DebugDrawSceneNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, s32 id)
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

void DebugDrawSceneNode::OnRegisterSceneNode()
{
	if(IsVisible) {
		SceneManager->registerNodeForRendering(this);
	}
	ISceneNode::OnRegisterSceneNode();
}

void DebugDrawSceneNode::render()
{
	IVideoDriver *driver = SceneManager->getVideoDriver();
	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(VertexList.data(), VertexList.size(), IndexList.data(), IndexList.size(), video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
	driver->draw2DVertexPrimitiveList(Vertex2DList.data(), Vertex2DList.size(), Index2DList.data(), Index2DList.size(), video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
}

void DebugDrawSceneNode::addLine(const vector_type &p1, const vector_type &p2, const color_type &color)
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

void DebugDrawSceneNode::addLine(const vector_2d_type &p1, const vector_2d_type &p2, const color_type &color)
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

void DebugDrawSceneNode::clear()
{
	VertexList.clear();
	IndexList.clear();
	Vertex2DList.clear();
	Index2DList.clear();
}