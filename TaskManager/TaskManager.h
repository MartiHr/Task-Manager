#pragma once
#include "User.h"
#include "Vector.hpp"

class TaskManager
{
private:
	static Vector<User> usersState;

	static void handleCommands(MyString& command, std::istream& is, const char* userDataFile);
	static void handleRegister(std::istream& is, const char* userDataFile);
	static void handleLogin(std::istream& is, const char* userDataFile);
public:
	static void start(std::istream& is, const char* );
};