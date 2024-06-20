#pragma once
#include <ctime>
#include "Vector.hpp"
#include "Task.h"

class Dashboard
{
private:
    std::time_t today;
    Vector<Task*> tasks;  // Member variable to store pointers to tasks
    bool isTaskDueToday(const Task& task) const;
public:
    Dashboard();

    // Set tasks for today
    void setTasks(Vector<Task*>& tasks);

    // Add a single task
    void addTask(Task& task);

    void free();

    // Get tasks for today
    Vector<Task*> getTasksForToday() const;
};
