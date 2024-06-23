#pragma once
#include <ctime>
#include "Vector.hpp"
#include "Task.h"
#include "MyString.h"

class Dashboard
{
private:
	MyString ownerUsername;
	Vector<int> taskIds;

	// too much overhead for the current example
	//Vector<Task*> tasks;  // Member variable to store pointers to tasks
public:
	Dashboard();
	Dashboard(const MyString& owner);

	void addTask(int id);
	void removeTask(int id);

	const MyString& getOwnerUsername() const;

	void populateDashboard(const Vector<int>& idsToAdd);
	void depopulateDashboard(const Vector<int>& idsToRemove);


	//void setTasks(Vector<Task*>& tasks);
	//void addTask(Task& task);
	//void free();
};
