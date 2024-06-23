#pragma once
#include "Vector.hpp"
#include "User.h";
#include "Task.h"
#include "Dashboard.h"
#include "TaskToUserMap.h"


class TaskCollectionManager
{
public:
    static Vector<User> usersState;
    static Vector<Task> tasks;
    static Vector<Dashboard> dashboards;
    static TaskToUserMap taskToUserMap;

    static Task& findTask(int taskId);
    static Task& findTask(const MyString& name);
};