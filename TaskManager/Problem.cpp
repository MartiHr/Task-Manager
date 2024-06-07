#include "Problem.h"

int Problem::lastUid = 0;

MyString statusToString(Status status) const
{
	switch (status)
	{
	case ON_HOLD: return "ON_HOLD";
		break;
	case IN_PROCESS: return "IN_PROCESS";
		break;
	case DONE: return "DONE";
	case OVERDUE: return "OVERDUE";
	default: return "UNKNOWN";
	}
}