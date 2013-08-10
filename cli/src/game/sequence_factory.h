// Ŭnicode please 
#pragma once

typedef enum {
	kSequenceTitle,
	kSequenceDebugDraw,
	kSequenceGame,
	kSequenceGameLoading,
	// demo라고 해서 다른 enum으로 분리하면 관리가 귀찮을듯
	kSequenceDemoCylinderHMD,
	kSequenceDemoSphereHMD,
} SequenceType;

class Sequence;

class SequenceFactory {
public:
	std::unique_ptr<Sequence> create(SequenceType type) const;
	SequenceType select() const;
};