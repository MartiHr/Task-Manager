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
	for (size_t i = 0; i < size; i++)
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
