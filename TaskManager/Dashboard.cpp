#pragma warning(disable:4996)
#include "Dashboard.h"
#include "Vector.hpp"
#include <algorithm>
#include <iostream>

// Helper function to compare if two dates are the same day
bool sameDay(const std::time_t& a, const std::time_t& b)
{
	std::tm tm_a = *std::localtime(&a);
	std::tm tm_b = *std::localtime(&b);
	return (tm_a.tm_year == tm_b.tm_year) &&
		(tm_a.tm_mon == tm_b.tm_mon) &&
		(tm_a.tm_mday == tm_b.tm_mday);
}

//bool Dashboard::isTaskDueToday(const Task& task) const
//{
//    const std::time_t* dueDate = task.getDueDate();
//    if (dueDate != nullptr)
//    {
//        return sameDay(*dueDate, today);
//    }
//
//    return false;
//}

Dashboard::Dashboard() {}

Dashboard::Dashboard(const MyString& owner)
{
	ownerUsername = owner;
}

//void Dashboard::setTasks(Vector<Task*>& tasks)
//{
//    this->tasks = tasks;
//}

//void Dashboard::addTask(Task& task)
//{
//    tasks.pushBack(&task);
//}

void Dashboard::addTask(int id)
{
	taskIds.pushBack(id);
}

void Dashboard::removeTask(int id)
{
	int indexToPopAt = -1;

	int size = taskIds.getSize();

	for (int i = 0; i < size; i++)
	{
		if (taskIds[i] == id)
		{
			indexToPopAt = i;
		}
	}

	taskIds.popAt(indexToPopAt);
}

const MyString& Dashboard::getOwnerUsername() const
{
	return ownerUsername;
}

const Vector<int>& Dashboard::getTaskIds() const
{
	return taskIds;
}

void Dashboard::populateDashboard(const Vector<int>& idsToAdd)
{
	for (int i = 0; i < idsToAdd.getSize(); i++)
	{
		addTask(idsToAdd[i]);
	}
}

void Dashboard::depopulateDashboard(const Vector<int>& idsToRemove)
{
	for (int i = 0; i < idsToRemove.getSize(); i++)
	{
		try
		{
			removeTask(idsToRemove[i]);
		}
		catch (const std::exception&)
		{
			continue;
		}
	}
}
