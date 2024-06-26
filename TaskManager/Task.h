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

	int getId() const;
	const MyString& getName() const;
	const std::time_t* getDueDate() const;
	Status getStatus() const;
	const MyString& getDescription() const;

	void setName(const MyString& newName);
	void setDescription(const MyString& newDescription);
	void setStatus(Status status);
private:
	int getNextUid() const;
};
