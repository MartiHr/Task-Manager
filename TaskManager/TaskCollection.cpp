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

void TaskCollection::removeTaskAt(int index)
{
	tasks.popAt(index);
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
	if (taskId < 0 || taskId >= tasks.getSize())
	{
		throw std::invalid_argument("Task does not exist");
	}

	return tasks[taskId];
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
