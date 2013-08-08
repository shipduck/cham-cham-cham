// Ŭnicode please 
#pragma once

typedef enum {
	kSequenceTitle,
	kSequenceDebugDraw,
	kSequenceGame,
	kSequenceGameLoading,
} SequenceType;

typedef enum {
	kDemoCylinderHMD,
} DemoSequenceType;

class Sequence;

class SequenceFactory {
public:
	std::unique_ptr<Sequence> create(SequenceType type) const;
	std::unique_ptr<Sequence> create(DemoSequenceType type) const;
};