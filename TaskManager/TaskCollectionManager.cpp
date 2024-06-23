#include "TaskCollectionManager.h"

Task& TaskCollectionManager::findTask(int taskId)
{
	if (taskId < 0 || taskId >= tasks.getSize())
	{
		throw std::invalid_argument("Task does not exist");
	}

	return tasks[taskId];
}

Task& TaskCollectionManager::findTask(const MyString& name)
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