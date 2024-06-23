#pragma once
#include <ctime>
#include "Vector.hpp"
#include "Task.h"
#include "MyString.h"

class Dashboard
{
private:
	std::time_t today;

	MyString ownerUsername;
	Vector<int> taskIds;

	// too much overhead for the current example
	//Vector<Task*> tasks;  // Member variable to store pointers to tasks
public:
	Dashboard();
	Dashboard(const MyString& owner);

	void addTask(int id);
	void removeTask(int id);

	Vector<Task*> getTasksForToday() const;

	const MyString& getOwnerUsername() const;

	//void setTasks(Vector<Task*>& tasks);
	//void addTask(Task& task);
	//void free();
private:
	bool isTaskDueToday(const Task& task) const;
};
