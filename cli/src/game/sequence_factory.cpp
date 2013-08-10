// Ŭnicode please 
#include "stdafx.h"
#include "sequence_factory.h"

#include "game_sequence.h"
#include "debug_draw_sequence.h"
#include "game_loading_sequence.h"
#include "title_sequence.h"

// demo
#include "demo/cylinder_hmd_sequence.h"
#include "demo/sphere_hmd_sequence.h"

using namespace std;

std::unique_ptr<Sequence> SequenceFactory::create(SequenceType type) const
{
	Sequence *seq = nullptr;
	switch(type) {
	case kSequenceTitle:
		seq = new TitleSequence();
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
	case kSequenceDemoCylinderHMD:
		seq = new CylinderHMDSequence();
		break;
	case kSequenceDemoSphereHMD:
		seq = new SphereHMDSequence();
		break;

	default:
		break;
	}
	SR_ASSERT(seq != nullptr);
	return unique_ptr<Sequence>(seq);
}

SequenceType SequenceFactory::select() const
{
	//키보드 입력을 받아서 특정 demo sequence를 생성한다든가
	//테스트 목족으로 sequence생성을 임의로 수정할떄
	//이부분이 계속 diff에 걸리면 변경추적이 어려우니까
	//작정하고 분리해놧음
	ostringstream oss;
	oss << "Select Sequence" << endl;
	oss << "(1) Demo Cylinder" << endl;
	oss << "(2) Demo Spehere" << endl;
	oss << "(3) Debug Draw" << endl;
	oss << "(etc) Title" << endl;
	cout << oss.str();

	char i;
	std::cin >> i;

	switch(i) {
	case '1':
		return kSequenceDemoCylinderHMD;
	case '2':
		return kSequenceDemoSphereHMD;
	case '3':
		return kSequenceDebugDraw;
	default:
		return kSequenceTitle;
	}
}