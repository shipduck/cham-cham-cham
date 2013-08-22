// Ŭnicode please 
#include "stdafx.h"
#include "sequence_factory.h"

#include "game_sequence.h"
#include "debug_draw_sequence.h"
#include "game_loading_sequence.h"
#include "main_sequence.h"

// demo
#include "demo/color_sequence.h"
#include "demo/ui_test_sequence.h"
#include "demo/ani_sequence.h"

using namespace std;

Sequence *SequenceFactory::createRaw(SequenceType type) const
{
	Sequence *seq = nullptr;
	switch(type) {
	//case kSequenceTitle:
	//	seq = new TitleSequence();
	//	break;
	case kSequenceMain:
		seq = new MainSequence();
		break;
	case kSequenceDebugDraw:
		seq = new DebugDrawSequence();
		break;
	case kSequenceGame:
		seq = new GameSequence();
		break;
	case kSequenceGameLoading:
		seq = new GameLoadingSequence();
		break;
	case kSequenceDemoColor:
		seq = new demo::ColorSequence();
		break;
	case kSequenceDemoAni:
		seq = new demo::AniSequence();
		break;
	default:
		break;
	}
	SR_ASSERT(seq != nullptr);
	return seq;
}
std::unique_ptr<Sequence> SequenceFactory::create(SequenceType type) const
{
	auto seq = createRaw(type);
	return unique_ptr<Sequence>(seq);
}

SequenceType SequenceFactory::select(DisplayType *displayType) const
{
	SR_ASSERT(displayType != nullptr);
	//키보드 입력을 받아서 특정 demo sequence를 생성한다든가
	//테스트 목족으로 sequence생성을 임의로 수정할떄
	//이부분이 계속 diff에 걸리면 변경추적이 어려우니까
	//작정하고 분리해놧음
	ostringstream oss;
	oss << "Select Sequence." << endl;
	oss << "If last character is f, run fullscreen mode." << endl;
	oss << "elif last character is w, run window mode." << endl;
	oss << "else, run saved option" << endl;
	cout << oss.str();

	string input;
	std::cin >> input;

	char code = input[0];
	char lastChar = input[input.length()-1];
	if(lastChar == 'f') {
		*displayType = kDisplayFullscreen;
	} else if(lastChar == 'w') {
		*displayType = kDisplayWindow;
	} else {
		*displayType = kDisplayDefault;
	}
	return kSequenceMain;
}