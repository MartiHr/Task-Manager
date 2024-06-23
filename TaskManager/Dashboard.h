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
	//Vector<Task*> tasks;  // Member variable to store pointers to tasks
public:
    Dashboard();
    Dashboard(const MyString& owner);

    const MyString& getOwnerUsername() const;
    const Vector<int>& getTaskIds() const; // Getter for task IDs

    void addTask(int id);
    void removeTask(int id);

    void populateDashboard(const Vector<int>& idsToAdd);
    void depopulateDashboard(const Vector<int>& idsToRemove);
};
