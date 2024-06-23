#pragma once
#include "User.h"
#include "Vector.hpp"
#include "Task.h"
#include "Dashboard.h"
#include "Pair.hpp"
#include "TaskToUserMap.h"
#include "CurrentUserState .h"
#include "UserCollection.h"
#include "TaskCollection.h"

// engine-like class
class TaskManager
{
private:
	// possibly extract the following three in another class 
	static UserCollection usersState;
	static TaskCollection tasks;
	// So as not to put additional fields in Task in order
	// to copmly with the problem criteria
	static TaskToUserMap taskToUserMap; // Mapping from task UID to username

	static Vector<Dashboard> dashboards;

	//TODO: extract in CurrentUserState class or something of the sort
	// for the following two
	static CurrentUserState currentUserState;

	static void handleCommands(std::istream& is, const char* userDataFile);
	
	//TODO: extract the following three is a sessionManager class
	static void handleRegister(std::istream& is, const char* userDataFile);
	static void handleLogin(std::istream& is);
	static void handleLogout();

	static void handleAddTask(std::istream& is);
	static void handleUpdateTaskName(std::istream& is);
	static void handleStartTask(std::istream& is);
	static void handleUpdateTaskDescription(std::istream& is);
	static void handleGetTask(std::istream& is);
	static void handleListTasks(std::stringstream& ss);
	static void handleFinishTask(std::istream& is);
	static void handleListCompletedTasks();
	static void handleDeleteTask(std::istream& is);

	static void handleAddTaskToDashboard(std::istream& is);
	static void handleRemoveTaskFromDashboard(std::istream& is);

public:
	static void start(std::istream& is, const char* userDataFile);
private:
	//static Task& findTask(int taskId);
	//static Task& findTask(const MyString& name);
	//static Vector<Task> getUserTasks(const MyString& username);

	static void setDashboardTasks(const MyString& username);

	static void listTasksByDate(const MyString& date);
	static void listAllTasks();

	// TODO: extract in utils class
	static void printDueDate(const std::time_t* dueDate);
	static const char* statusToString(Status status);
	static void printTask(const Task& task);

};