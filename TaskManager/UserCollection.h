#pragma once
#include "Vector.hpp"
#include "User.h"
#include "MyString.h"

class UserCollection
{
private:
	Vector<User> usersState;
public:
	void addUser(const User& user);
	const Vector<User>& getUsers() const;

	bool checkUserExists(const MyString& loginUsername, const MyString& loginPassword) const;

};