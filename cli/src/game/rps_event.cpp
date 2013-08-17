// Ŭnicode please 
#include "stdafx.h"
#include "rps_event.h"

bool RPSEvent::operator==(const RPSEvent &o) const
{
	SR_ASSERT(o.value != kNone);
	SR_ASSERT(this->value != kNone);
	return this->value == o.value;
}

bool RPSEvent::operator>(const RPSEvent &o) const
{
	SR_ASSERT(o.value != kNone);
	SR_ASSERT(this->value != kNone);
	if(this->value == kRock && o.value == kScissor) {
		return true;
	}
	if(this->value == kScissor && o.value == kPaper) {
		return true;
	}
	if(this->value == kPaper && o.value == kRock) {
		return true;
	}
	return false;
}

bool RPSEvent::operator>=(const RPSEvent &o) const
{
	if(*this == o) {
		return true;
	}
	return (*this > o);
}

bool RPSEvent::operator<(const RPSEvent &o) const
{
	SR_ASSERT(o.value != kNone);
	SR_ASSERT(this->value != kNone);
	if(this->value == kScissor && o.value == kRock) {
		return true;
	}
	if(this->value == kPaper && o.value == kScissor) {
		return true;
	}
	if(this->value == kRock && o.value == kPaper) {
		return true;
	}
	return false;
}
bool RPSEvent::operator<=(const RPSEvent &o) const
{
	if(*this == o) {
		return true;
	}
	return (*this < o);
}
