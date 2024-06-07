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

class Problem
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
	Problem();
private:
	MyString statusToString(Status status) const;
	int getNextUid() const;
};
