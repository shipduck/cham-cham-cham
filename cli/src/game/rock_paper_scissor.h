// Ŭnicode please 
#pragma once

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
	bool isEnable() const;

	void update(int ms);

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
};