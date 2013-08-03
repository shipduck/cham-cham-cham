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
	
}	// namespace hmd_ui
