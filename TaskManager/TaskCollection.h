#pragma once
#include "Vector.hpp"
#include "Task.h"
#include "MyString.h"

// Would be good to inherit a base colleciton class
class TaskCollection
{
private:
	Vector<Task> tasks;
public:
	//void setTasks(const Vector<Task>& users);
	const Vector<Task>& getTasks() const;

	void addTask(const Task& task);
	void updateTaskName(int id, const MyString& newName);
	void startTask(int id);
	void updateTaskDescription(int id, const MyString& newDesc);
	// allowed changing the returned task
	Task& findTask(int taskId);
	Task& findTask(const MyString& name);
	void finishTask(int id);

	void removeTask(int id);

	Vector<int> getTaskIdsForToday() const;

private:
	bool checkUnique(const Task& task) const;
	bool isTaskDueToday(const Task& task) const;
};