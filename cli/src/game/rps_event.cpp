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
	if(o == loseEvent()) {
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
	if(o == winEvent()) {
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

RPSEvent RPSEvent::winEvent() const
{
	int other = 0;
	switch(value) {
	case kRock:
		other = kPaper;
		break;
	case kPaper:
		other = kScissor;
		break;
	case kScissor:
		other = kRock;
		break;
	default:
		SR_ASSERT(!"do not reach");
	}
	return RPSEvent(other);
}
RPSEvent RPSEvent::loseEvent() const
{
	int other = 0;
	switch(value) {
	case kRock:
		other = kScissor;
		break;
	case kPaper:
		other = kRock;
		break;
	case kScissor:
		other = kPaper;
		break;
	default:
		SR_ASSERT(!"do not reach");
	}
	return RPSEvent(other);
}
RPSEvent RPSEvent::drawEvent() const
{
	return *this;
}