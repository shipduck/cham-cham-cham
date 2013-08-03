// Ŭnicode please 
#pragma once

typedef enum {
	kSequenceTitle,
	kSequenceDebugDraw,
	kSequenceGame,
	kSequenceGameLoading,
} SequenceType;

class Sequence;

class SequenceFactory {
public:
	std::unique_ptr<Sequence> create(SequenceType type) const;
};