#pragma once
#include <ctime>
#include "Vector.hpp"
#include "Task.h"
#include "MyString.h"

class Dashboard
{
private:
    std::time_t today;
    Vector<Task*> tasks;  // Member variable to store pointers to tasks

    MyString ownerUsername;
    Vector<int> taskIds;
public:
    Dashboard();

    // Set tasks for today
    void setTasks(Vector<Task*>& tasks);

    // Add a single task
    void addTask(Task& task);

    // Get tasks for today
    Vector<Task*> getTasksForToday() const;
    
    void free();
private:
    bool isTaskDueToday(const Task& task) const;
};
