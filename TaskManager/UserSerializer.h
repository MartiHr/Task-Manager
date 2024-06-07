#pragma once

#include "User.h"
class UserSerializer
{
public:
	static void saveUser(const User& user, const char* fileName);
	static User readUser(const char* fileName);

};