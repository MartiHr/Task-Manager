#pragma once
#include "Vector.hpp"
#include "User.h"
#include "MyString.h"

class UserCollection
{
private:
    Vector<User*> usersState; // Store pointers to User objects

public:
    void setUsers(const Vector<User*>& users);
    void addUser(User* user); // Accept a pointer to User
    const Vector<User*>& getUsers() const;

    bool checkUserExists(const MyString& loginUsername, const MyString& loginPassword) const;
    
    ~UserCollection();
};