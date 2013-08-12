// Ŭnicode please 
#include "stdafx.h"
#include "color_sequence.h"
#include "base/lib.h"

namespace demo {;

ColorSequence::ColorSequence()
{
}
ColorSequence::~ColorSequence()
{
}

void ColorSequence::update(int ms)
{
	Lib::backgroundColor = irr::video::SColor(255, 255, 255, 255);
}

}	// namespace demo