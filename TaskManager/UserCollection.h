#pragma once
#include "Vector.hpp"
#include "User.h"
#include "MyString.h"

// Would be good to inherit a base colleciton class
class UserCollection
{
private:
	Vector<User> usersState;
public:
	void setUsers(const Vector<User>& users);
	
	void addUser(const User& user);
	const Vector<User>& getUsers() const;

	bool checkUserExists(const MyString& loginUsername, const MyString& loginPassword) const;
};