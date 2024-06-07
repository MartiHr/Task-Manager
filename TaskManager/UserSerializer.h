#pragma once

#include "User.h"
#include "Vector.hpp"

class UserSerializer
{
private:
	static User readUser(std::ifstream& file);
public:
	static void saveUser(const User& user, const char* fileName);
	static Vector<User> readUsers(const char* fileName);
	static bool ensureDataFileCreated(const char* fileName);
};