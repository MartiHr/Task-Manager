#include "Task.h"

int Task::lastUid = -1;

Task::Task() : uid(-1), status(Status::ON_HOLD) {}

Task::Task(const MyString& name, Status status, const MyString& description)
	: uid(getNextUid()), name(name), status(status), description(description) {}

Task::Task(const MyString& name, const Optional<std::time_t>& dueDate,
	Status status, const MyString& description)
	: uid(getNextUid()), name(name), dueDate(dueDate),
	status(status), description(description) {}

int Task::getId() const
{
	return uid;
}

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

Status Task::getStatus() const
{
	return status;
}

const MyString& Task::getDescription() const
{
	return description;
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

int Task::getNextUid() const
{
	return ++lastUid;
}
