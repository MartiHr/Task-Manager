#pragma once
#include "User.h"
#include "Vector.hpp"
#include "Task.h"
#include "Dashboard.h"

// engine-like class
class TaskManager
{
private:
	static Vector<User> usersState;
	static Vector<Task> tasks;
	static Dashboard dashboard;

	static bool loggedIn;

	static void handleCommands(MyString& command, std::istream& is, const char* userDataFile);
	
	static void handleRegister(std::istream& is, const char* userDataFile);
	static void handleLogin(std::istream& is);
	static void handleLogout();

	static void handleAddTask(std::istream& is);
	static void handleUpdateTaskName(std::istream& is);
	static void handleStartTask(std::istream& is);
	static void handleUpdateTaskDescription(std::istream& is);

	static void handleRemoveTaskFromDashboard(std::istream& is);
	static void handleAddTaskFromDashboard(std::istream& is);
	
	static void handleDeleteTask(std::istream& is);
public:
	static void start(std::istream& is, const char* userDataFile);

private:
	static Task& findTask(int taskId);
	static Task& findTask(const MyString& name);
};