#include "TaskToUserMap.h"

void TaskToUserMap::addMapping(int taskUid, const MyString& username)
{
    taskUids.pushBack(taskUid);
    usernames.pushBack(username);
}

void TaskToUserMap::removeMapping(int taskUid, const MyString& username)
{
    Vector<int> newTaskUids;
    Vector<std::string> newUsernames;

    for (size_t i = 0; i < taskUids.getSize(); ++i) 
    {
        if (taskUids[i] != taskUid || usernames[i] != username)
        {
            newTaskUids.pushBack(taskUids[i]);
            newUsernames.pushBack(usernames[i]);
        }
    }

    taskUids = newTaskUids;
    usernames = newUsernames;
}

Vector<int> TaskToUserMap::getTasksForUser(const MyString& username) const
{
    Vector<int> tasksForUser;

    for (size_t i = 0; i < usernames.getSize(); ++i) 
    {
        if (usernames[i] == username)
        {
            tasksForUser.pushBack(taskUids[i]);
        }
    }

    return tasksForUser;
}

Vector<std::string> TaskToUserMap::getUsersForTask(int taskUid) const
{
    Vector<std::string> usersForTask;

    for (size_t i = 0; i < taskUids.getSize(); ++i)
    {
        if (taskUids[i] == taskUid) 
        {
            usersForTask.pushBack(usernames[i]);
        }
    }

    return usersForTask;
}

void TaskToUserMap::clearMappingsForUser(const MyString& username)
{
    Vector<int> newTaskUids;
    Vector<std::string> newUsernames;

    for (size_t i = 0; i < taskUids.getSize(); ++i) 
    {
        if (usernames[i] != username) 
        {
            newTaskUids.pushBack(taskUids[i]);
            newUsernames.pushBack(usernames[i]);
        }
    }

    taskUids = newTaskUids;
    usernames = newUsernames;
}
