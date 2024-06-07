#include "Problem.h"

int Problem::lastUid = 0;

MyString Problem::statusToString(Status status) const
{
	switch (status)
	{
	case Status::ON_HOLD:
		return "ON_HOLD";
		break;
	case Status::IN_PROCESS:
		return "IN_PROCESS";
		break;
	case Status::DONE: return
		"DONE";
		break;
	case Status::OVERDUE:
		return "OVERDUE";
		break;
	default: return "UNKNOWN";
		break;
	}
}
