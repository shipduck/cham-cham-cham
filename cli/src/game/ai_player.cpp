// Ŭnicode please 
#include "stdafx.h"
#include "ai_player.h"
#include "rps_event.h"
#include "finger_direction_event.h"

using namespace std;
using namespace irr;

AiPlayer::AiPlayer()
{
}
AiPlayer::~AiPlayer()
{
}

RPSEvent AiPlayer::think(const RPSEvent &o) const
{
	//비길 확률을 극단적으로 낮추자
	//AI질:AI이길:비길 = 5:3:2
	float aiLose = 0.5f;
	float aiWin = aiLose + 0.3f;
	
	float val = generateRandomFloat();
	if(val <= aiLose) {
		return o.loseEvent();
	} else if(val <= aiWin) {
		return o.winEvent();
	} else {
		return o.drawEvent();
	}
}

FingerDirectionEvent AiPlayer::think(const FingerDirectionEvent &o) const
{
	//성공:실패=5:5
	//실패시 나머지 3개중에서 뭐 선택할지는 알아서 적절히
	float successRand = generateRandomFloat();
	if(successRand <= 0.5f) {
		return o;
	}
	std::vector<int> otherList;
	for(int i = 1 ; i <= 4 ; ++i) {
		if(i != o.value) {
			otherList.push_back(i);
		}
	}
	std::random_shuffle(otherList.begin(), otherList.end());
	return FingerDirectionEvent(otherList.front());
}

float AiPlayer::generateRandomFloat() const
{
	static std::default_random_engine e1((int)time(NULL));
	std::uniform_real_distribution<float> randGen(0, 1.0f);
	return randGen(e1);
}