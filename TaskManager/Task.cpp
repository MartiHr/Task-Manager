#include "Task.h"

int Task::lastUid = 0;

Task::Task() : uid(getNextUid()), status(Status::ON_HOLD) {}

Task::Task(const MyString& name, Status status, const MyString& description)
	: uid(getNextUid()), name(name), status(status), description(description) {}

Task::Task(const MyString& name, const Optional<std::time_t>& dueDate,
	Status status, const MyString& description)
	: uid(getNextUid()), name(name), dueDate(dueDate),
	status(status), description(description) {}

const MyString& Task::getName() const
{
	return name;
}

const std::time_t* Task::getDueDate() const
{
	if (dueDate.hasValue()) 
	{
		return &(dueDate.getValue());
	}
	else 
	{
		throw std::runtime_error("Due date is not set.");
	}
}

void Task::setName(const MyString& newName)
{
	name = newName;
}

void Task::setDescription(const MyString& newDescription)
{
	description = newDescription;
}

void Task::setStatus(Status status)
{
	this->status = status;
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
