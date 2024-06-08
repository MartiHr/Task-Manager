#include "Task.h"

int Task::lastUid = 0;

Task::Task(MyString& name, Optional<std::time_t>, Status status, MyString& description)
{

}

MyString Task::statusToString(Status status) const
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

int Task::getNextUid() const
{
	return ++lastUid;
}
