// Ŭnicode please 
#pragma once

namespace irr {;
namespace scene {;
class SpriteSceneNode;
}	// namespace scene
}	// namespace irr

class RPSEvent {
public:
	enum {
		kNone,
		kRock,
		kPaper,
		kScissor,
	};

	int value;

	RPSEvent() : value(RPSEvent::kNone) {}
	RPSEvent(int value) : value(value) {}

	static RPSEvent rock() { return RPSEvent(kRock); }
	static RPSEvent paper() { return RPSEvent(kPaper); }
	static RPSEvent scissor() { return RPSEvent(kScissor); }

	bool operator>(const RPSEvent &o) const;
	bool operator>=(const RPSEvent &o) const;
	bool operator<(const RPSEvent &o) const;
	bool operator<=(const RPSEvent &o) const;
	bool operator==(const RPSEvent &o) const;
};

class RPSEventReceiver;

class RockPaperScissor {
public:
	RockPaperScissor(irr::scene::ICameraSceneNode *cam);
	virtual ~RockPaperScissor();

	void setEnable(bool b);
	bool isEnable() const { return enable_; }

	void update(int ms);

	const RPSEvent &getAiChoice() const { return aiChoice_; }
	const RPSEvent &getPlayerChoice() const { return playerChoice_; }
	bool isEnd() const { return end_; }

private:
	void attachEventReceiver();
	void detachEventReceiver();

	RPSEvent selectAIEvent() const;

private:
	irr::scene::ISceneNode *root_;

	irr::scene::ISceneNode *rock_;
	irr::scene::ISceneNode *paper_;
	irr::scene::ISceneNode *scissor_;

	RPSEventReceiver *evtReceiver_;

private:
	// 가위바위보 결과를 임시 저장
	RPSEvent aiChoice_;
	RPSEvent playerChoice_;
	bool end_;

	bool enable_;
};
