// Ŭnicode please
#pragma once

namespace irr {;
namespace scene {;
class Text3dSceneNode;
}	// namespace scene
}	// namespace irr

class FingerDirectionEvent;
class FingerDirectionEventReceiver;

class BaseChamChamCham {
public:
	BaseChamChamCham(irr::scene::ICameraSceneNode *cam);
	virtual ~BaseChamChamCham();

	virtual void update(int ms) = 0;

	bool isEnd() const { return end_; }

	const FingerDirectionEvent &getAiChoice() const { return *aiChoice_; }
	const FingerDirectionEvent &getPlayerChoice() const;

protected:
	bool end_;
	
	irr::scene::ISceneNode *root_;
	irr::scene::Text3dSceneNode *centerText_;

	FingerDirectionEvent choiceAIEvent() const;
	std::unique_ptr<FingerDirectionEvent> aiChoice_;

	FingerDirectionEventReceiver *evtReceiver_;
};

class ChamChamChamAttack : public BaseChamChamCham {
public:
	ChamChamChamAttack(irr::scene::ICameraSceneNode *cam);
	virtual ~ChamChamChamAttack();

	void update(int ms);

};

class ChamChamChamDefense : public BaseChamChamCham {
public:
	ChamChamChamDefense(irr::scene::ICameraSceneNode *cam);
	virtual ~ChamChamChamDefense();

	void update(int ms);
private:
	
};