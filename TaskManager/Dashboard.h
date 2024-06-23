#pragma once
#include <ctime>
#include "Vector.hpp"
#include "Task.h"
#include "MyString.h"

class Dashboard
{
private:
    std::time_t today;
    Vector<Task*> tasks;  // Member variable to store pointers to tasks

    MyString ownerUsername;
    Vector<int> taskIds;
public:
    Dashboard();
    Dashboard(const MyString& owner);

    void setTasks(Vector<Task*>& tasks);

    //void addTask(Task& task);
    void addTask(int id);
    void addTask(int id);

    Vector<Task*> getTasksForToday() const;
    
    const MyString& getOwnerUsername() const;

    void free();
private:
    bool isTaskDueToday(const Task& task) const;
};
