#pragma once
#include <ctime>
#include "MyString.h"
#include "Optional.hpp"

enum class Status
{
	ON_HOLD,
	IN_PROCESS,
	DONE,
	OVERDUE
};

class Task
{
private:
	//TODO: add snapshot for problem names
	static int lastUid;

	int uid;
	MyString name;
	Optional<std::time_t> dueDate;
	Status status;
	MyString description;
public:
	Task();
	Task(const MyString& name, Status status, const MyString& description);
	Task(const MyString& name, const Optional<std::time_t>& dueDate,
		Status status, const MyString& description);

	MyString& getName() const;

	void setName(const MyString& newName);
	void setDescription(const MyString& newDescription);
	void setStatus(Status status);
private:
	MyString statusToString(Status status) const;
	int getNextUid() const;
};
