// Ŭnicode please
#pragma once

namespace irr {;
namespace scene {;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class ChamChamChamEvent {
public:
	enum {
		kNone,
		kUp,
		kDown,
		kLeft,
		kRight,
	};

	ChamChamChamEvent() : value(kNone) {}
	ChamChamChamEvent(int val) : value(val) {}

	static ChamChamChamEvent up() { return ChamChamChamEvent(kUp); }
	static ChamChamChamEvent down() { return ChamChamChamEvent(kDown); }
	static ChamChamChamEvent left() { return ChamChamChamEvent(kLeft); }
	static ChamChamChamEvent right() { return ChamChamChamEvent(kRight); }

	bool operator==(const ChamChamChamEvent &o) const;
	bool operator!=(const ChamChamChamEvent &o) const;

	int value;
};

class BaseChamChamCham {
public:
	BaseChamChamCham(irr::scene::ICameraSceneNode *cam);
	virtual ~BaseChamChamCham();

	virtual void update(int ms) = 0;

	bool isEnd() const { return end_; }

protected:
	bool end_;
	
	irr::scene::ISceneNode *root_;
	irr::scene::Text3dSceneNode *centerText_;
};

class ChamChamChamAttack : public BaseChamChamCham {
public:
	ChamChamChamAttack(irr::scene::ICameraSceneNode *cam);
	virtual ~ChamChamChamAttack() {}

	void update(int ms);
};

class ChamChamChamDefense : public BaseChamChamCham {
public:
	ChamChamChamDefense(irr::scene::ICameraSceneNode *cam);
	virtual ~ChamChamChamDefense() {}

	void update(int ms);
};