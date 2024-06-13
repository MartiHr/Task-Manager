#include "Dashboard.h"
#pragma warning(disable : 4996)

Dashboard::Dashboard()
{;
	today = std::time(nullptr); // Current time
}

void Dashboard::setTasks(Vector<Task> tasks) {
	tasksForToday = tasks;
}

void Dashboard::addTask(const Task& task)
{
	if (isTaskDueToday(task))
	{
		tasksForToday.pushBack(task);
	}
}

Vector<Task> Dashboard::getTasksForToday() const
{
	return tasksForToday;
}

bool Dashboard::isTaskDueToday(const Task& task)
{
	try
	{
		std::tm* taskDueDate = std::localtime(task.getDueDate());
		std::tm* todayDate = std::localtime(&today);

		return (taskDueDate->tm_year == todayDate->tm_year &&
			taskDueDate->tm_yday == todayDate->tm_yday);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
