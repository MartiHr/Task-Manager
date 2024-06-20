#pragma once 
#include <ctime>
#include "Vector.hpp"
#include "Task.h"

class Dashboard
{
private:
	Vector<Task> tasksForToday;
	std::time_t today;

    bool isTaskDueToday(const Task& task);
public:
    Dashboard();

    // Set tasks for today
    void setTasks(Vector<Task> tasks);

    // Add a single task
    void addTask(const Task& task);

    void free();

    // Get tasks for today
    Vector<Task> getTasksForToday() const;
};