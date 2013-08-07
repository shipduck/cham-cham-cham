// Ŭnicode please 
#pragma once

typedef enum {
	kDemoCylinderHMD,
} DemoSequenceType;

class Sequence;

class DemoFactory {
public:
	std::unique_ptr<Sequence> create(DemoSequenceType type) const;
};