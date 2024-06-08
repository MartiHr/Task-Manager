#pragma once
#include "User.h"
#include "Vector.hpp"

// engine-like class
class TaskManager
{
private:
	static Vector<User> usersState;

	static bool loggedIn;

	static void handleCommands(MyString& command, std::istream& is, const char* userDataFile);
	
	static void handleRegister(std::istream& is, const char* userDataFile);
	static void handleLogin(std::istream& is);
	static void handleLogout();

	static void handleAddTask(std::istream& is);
public:
	static void start(std::istream& is, const char* userDataFile);
};