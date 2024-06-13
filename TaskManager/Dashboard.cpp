#include "Dashboard.h"

Dashboard::Dashboard() 
{
    today = std::time(nullptr); // Current time
}

void Dashboard::setTasks(Vector<Task> tasks) {
    tasksForToday = tasks;
}

void Dashboard::addTask(const Task& task) 
{
    if (isTaskDueToday(task)) 
    {
        tasksForToday.push_back(task);
    }
}

Vector<Task> Dashboard::getTasksForToday() const
{
    return tasksForToday;
}

bool Dashboard::isTaskDueToday(const Task& task)
{
    // Assuming Task class has a method to get due date as time_t
    std::tm* taskDueDate = std::localtime(&task.getDueDate());
    std::tm* todayDate = std::localtime(&today);

    return (taskDueDate->tm_year == todayDate->tm_year &&
        taskDueDate->tm_yday == todayDate->tm_yday);
}
