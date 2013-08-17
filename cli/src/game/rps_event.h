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

	RPSEvent winEvent() const;
	RPSEvent loseEvent() const;
	RPSEvent drawEvent() const;
};
