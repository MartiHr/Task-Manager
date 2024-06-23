#pragma warning(disable:4996)
#include "TaskCollection.h"

void TaskCollection::addTask(const Task& task)
{
	if (!checkUnique(task))
	{
		throw std::exception("Task with already exists");
	}

	tasks.pushBack(task);
}

void TaskCollection::updateTaskName(int id, const MyString& newName)
{
	Task& taskToChange = findTask(id);
	taskToChange.setName(newName);
}

void TaskCollection::startTask(int id)
{
	Task& taskToChange = findTask(id);
	taskToChange.setStatus(Status::IN_PROCESS);
}

void TaskCollection::updateTaskDescription(int id, const MyString& newDesc)
{
	Task& taskToChange = findTask(id);
	taskToChange.setDescription(newDesc);
}

void TaskCollection::removeTask(int id)
{
	int size = tasks.getSize();

	for (int i = 0; i < size; i++)
	{
		if (tasks[i].getId() == id)
		{
			tasks.popAt(i);
			return;
		}
	}

	throw std::exception("No task with the provided id exists");
}
	
bool sameDay(std::time_t time1, std::time_t time2)
{
	// Convert time_t values to tm structs for day comparison
	std::tm tm1 = *std::localtime(&time1);
	std::tm tm2 = *std::localtime(&time2);

	// Compare year, month, and day fields
	return (tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon && tm1.tm_mday == tm2.tm_mday);
}

bool TaskCollection::isTaskDueToday(const Task& task) const
{
	const std::time_t* dueDate = task.getDueDate();
	if (dueDate != nullptr)
	{
		std::time_t currentTime = std::time(nullptr);
		return sameDay(*dueDate, currentTime);
	}

	return false;
}

Vector<int> TaskCollection::getTaskIdsForToday() const
{
	Vector<int> taskIdsForToday;

	for (int i = 0; i < tasks.getSize(); i++)
	{
		if (isTaskDueToday(tasks[i]))
		{
			taskIdsForToday.pushBack(tasks[i].getId());
		}
	}
	
	return taskIdsForToday;
}

bool isTaskExpired(const Task& task)
{
	const std::time_t* dueDate = task.getDueDate();
	if (dueDate != nullptr)
	{
		std::time_t currentTime = std::time(nullptr); // Get current time
		return (*dueDate < currentTime);
	}

	return false;
}

Vector<int> TaskCollection::getExpiredTaskIds() const
{
	Vector<int> expiredTaskIds;

	for (int i = 0; i < tasks.getSize(); i++)
	{
		if (isTaskExpired(tasks[i]))
		{
			expiredTaskIds.pushBack(tasks[i].getId());
		}
	}

	return expiredTaskIds;
}

bool TaskCollection::checkUnique(const Task& task) const
{
	int size = tasks.getSize();

	for (int i = 0; i < size; i++)
	{
		if (task.getId() == tasks[i].getId()
			|| (task.getName() == tasks[i].getName()
				&& *(task.getDueDate()) == *(tasks[i].getDueDate())))
		{
			return false;
		}
	}

	return true;
}


Task& TaskCollection::findTask(int taskId)
{
	int size = tasks.getSize();
	for (int i = 0; i < size; i++)
	{
		if (tasks[i].getId() == taskId)
		{
			return tasks[i];
		}
	}

	throw std::invalid_argument("Task does not exist");
}

Task& TaskCollection::findTask(const MyString& name)
{
	//find the first task with id with the provided name
	for (int i = 0; i < tasks.getSize(); i++)
	{
		if (tasks[i].getName() == name)
		{
			return tasks[i];
		}
	}

	throw std::invalid_argument("Task does not exist");
}

void TaskCollection::finishTask(int id)
{
	Task& taskToChange = findTask(id);
	taskToChange.setStatus(Status::DONE);
}
