// Ŭnicode please 
#pragma once

class RPSEvent;
class FingerDirectionEvent;

class AiPlayer {
public:
	AiPlayer();
	~AiPlayer();

	RPSEvent think(const RPSEvent &o) const;
	FingerDirectionEvent think(const FingerDirectionEvent &o) const;

private:
	float generateRandomFloat() const;
};