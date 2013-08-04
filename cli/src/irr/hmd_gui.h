// Ŭnicode please 
#pragma once

namespace hmd_ui {;

class Manager;
class Component;

class Manager {
public:
	Manager();
	~Manager();

	Component *add(Component *comp);
	void drawAll();

	void shutDown();

private:
	std::vector<Component*> compList_;
};

class Component {
public:
	Component(int w, int h, const std::string &name);
	virtual ~Component();

	//RT를 잡았다놨다 하는 함수를 draw에 때려박으면
	//component을 연속으로 그리는 함수를 도입할때 최적화하기 미묘할거같아서
	//분리시켜놨다
	void bind();
	static void unbind();
	// draw scene into render target
	void draw();

public:
	irr::video::ITexture *getTexture() { return rt_; }
	const irr::video::ITexture *getTexture() const { return rt_; }
	const irr::core::dimension2d<irr::u32> getSize() const { return rtSize_; }
	irr::scene::ISceneManager *getSceneMgr() { return smgr_; }
	const irr::scene::ISceneManager *getSceneMgr() const { return smgr_; }
	void setBackgroundColor(const irr::video::SColor &val) { bgColor_ = val; }
	const irr::video::SColor &getBackgroundColor() const { return bgColor_; }

private:
	// 렌더 텍스쳐에 그릴 내용을 기본smgr공유해서 사용하면 골치아프니까 분리
	irr::scene::ISceneManager *smgr_;
	irr::video::ITexture *rt_;
	irr::core::dimension2d<irr::u32> rtSize_;
	irr::video::SColor bgColor_;
};

class TestComp: public Component {
public:
	TestComp();
	virtual ~TestComp();
};


class CylinderMappingNode : public irr::scene::ISceneNode {
public:
	typedef irr::video::S3DVertex vertex_type;
	typedef std::vector<vertex_type> vertex_list_type;
	typedef std::vector<unsigned short> index_list_type;

public:
	CylinderMappingNode(irr::scene::ISceneNode *parent, irr::scene::ISceneManager *smgr, irr::s32 id);
	virtual ~CylinderMappingNode();

	virtual void OnRegisterSceneNode();
	virtual void render() = 0;
	virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const { return box_; }
	virtual irr::u32 getMaterialCount() const { return 1; }
	virtual irr::video::SMaterial &getMaterial(irr::u32 i) { return material_; }

	// node를 구성하는 내용의 값이 바뀐경우 호출해서 새로운 vertex/index list등을 생성하는 함수
	// 자동으로 호출되도록 바꿀수도 있겠지만 일단은 구조를 간단하게 유지하기 위해서 수동호출
	virtual void rebuild() = 0;

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
};

class CylinderTextureNode : public CylinderMappingNode {
public:
	CylinderTextureNode(irr::scene::ISceneNode *parent, 
		irr::scene::ISceneManager *smgr, 
		irr::s32 id,
		irr::video::ITexture *tex);
	virtual ~CylinderTextureNode();
	virtual void render();
	virtual void rebuild();

private:
	std::vector<vertex_type> vertexList_;
	std::vector<unsigned short> indexList_;
	irr::video::ITexture *tex_;
};

class CylinderButtonNode : public CylinderMappingNode {
public:
	CylinderButtonNode(irr::scene::ISceneNode *parent, 
		irr::scene::ISceneManager *smgr, 
		irr::s32 id,
		const char *normalFile,
		const char *selectFile = nullptr);	
	virtual ~CylinderButtonNode();
	virtual void render();
	virtual void rebuild();

public:
	bool selected;

private:
	irr::video::ITexture *normalTex_;
	irr::video::ITexture *selectTex_;

	std::vector<vertex_type> vertexList_;
	std::vector<unsigned short> indexList_;

	irr::video::ITexture *getTexture();
};

}	// namespace hmd_ui
