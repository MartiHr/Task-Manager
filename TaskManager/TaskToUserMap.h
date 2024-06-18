#pragma once
#include "Vector.hpp"
#include "MyString.h"

class TaskToUserMap
{
private:
    Vector<int> taskUids;
    Vector<MyString> usernames;
public:
    void addMapping(int taskUid, const MyString& username);
    void removeMappingByTaskId(int taskUid);
    void removeMapping(int taskUid, const MyString& username);
    Vector<int> getTasksForUser(const MyString& username) const;
    Vector<MyString> getUsersForTask(int taskUid) const;
    void clearMappingsForUser(const MyString& username);
};
