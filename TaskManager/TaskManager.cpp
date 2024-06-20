#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "TaskManager.h"
#include "MyString.h"
#include "UserSerializer.h"
#include "Pair.hpp"

Vector<User> TaskManager::usersState;
Vector<Task> TaskManager::tasks;
Dashboard TaskManager::dashboard;
TaskToUserMap TaskManager::taskToUserMap;

bool TaskManager::loggedIn = false;

void TaskManager::handleCommands(MyString& command, std::istream& is, const char* userDataFile)
{
	// TODO: extract string elsewhere
	while (command != "exit")
	{
		if (command == "register")
		{
			handleRegister(is, userDataFile);
		}
		else if (command == "login")
		{
			handleLogin(is);
		}
		else if (command == "logout")
		{
			handleLogout();
		}
		else if (command == "add-task")
		{
			handleAddTask(is);
		}
		else if (command == "update-task-name")
		{
			handleUpdateTaskName(is);
		}
		else if (command == "start-task")
		{
			handleStartTask(is);
		}
		else if (command == "update-task-description")
		{
			handleUpdateTaskDescription(is);
		}
		else if (command == "remove-task-from-dashboard")
		{

		}
		else if (command == "add-task-to-dashboard")
		{

		}
		else if (command == "delete-task")
		{
			handleDeleteTask(is);
		}
		else if (command == "get-task")
		{
			handleGetTask(is);
		}
		else if (command == "list-tasks")
		{
			handleListTasks(is);
		}
		else if (command == "list-completed-tasks")
		{
			handleListCompletedTasks();
		}
		else if (command == "list-dashboard")
		{

		}
		else if (command == "finish-task")
		{
			// check whether additional logic is needed for the funciton
			handleFinishTask(is);
		}
		else if (command == "exit")
		{
			break;
		}
		else
		{
			std::cout << "No such command." << std::endl;
		}

		std::cin >> command;
	}
}

void TaskManager::handleRegister(std::istream& is, const char* userDataFile)
{
	if (loggedIn)
	{
		std::cout << "Already logged in" << std::endl;
		return;
	}

	MyString username;
	MyString password;

	is >> username;
	is >> password;

	// add to binary file
	User currentUser = { username, password };
	UserSerializer::saveUser(currentUser, userDataFile);

	// add to state
	usersState.pushBack(currentUser);

	std::cout << "Registered user " << username << " successfully!" << std::endl;
}

void TaskManager::handleLogin(std::istream& is)
{
	if (loggedIn)
	{
		std::cout << "Already logged in" << std::endl;
		return;
	}

	MyString loginUsername;
	MyString loginPassword;

	is >> loginUsername;
	is >> loginPassword;

	for (int i = 0; i < usersState.getSize(); i++)
	{
		if (usersState[i].username == loginUsername && usersState[i].password == loginPassword)
		{
			std::cout << "Welcome back, " << loginUsername << "!" << std::endl;
			loggedIn = true;

			// load the dashboard of the user
			//dashboard.setTasks(loginUsername);
			dashboard.setTasks()
			break;
		}
		else
		{
			std::cout << "Wrong credentials." << std::endl;
		}
	}
}

void TaskManager::handleLogout()
{
	loggedIn = false;
	//TODO: vector may not work as intended here !!!
	dashboard.free();
	std::cout << "Logged out" << std::endl;
}

Optional<std::time_t> parseDate(const MyString& dateStr)
{
	if (dateStr.getSize() == 0)
	{
		return Optional<std::time_t>(); // Return empty optional if no date is provided
	}

	std::tm tm = {};

	// Used to validate input date format
	std::istringstream ss(dateStr.c_str());

	ss >> std::get_time(&tm, "%Y-%m-%d");
	if (ss.fail())
	{
		throw std::runtime_error("Invalid date format. Use YYYY-MM-DD.");
	}

	return std::mktime(&tm);
}

void TaskManager::handleAddTask(std::istream& is)
{
	MyString name, description, dateStr;
	is >> name;
	is >> dateStr;
	is.ignore(); // Ignore any remaining newline character

	// Clear the newline character left in the input buffer by previous >> operations
	is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	char descriptionBuffer[1024];
	is.getline(descriptionBuffer, 1024);
	description = MyString(descriptionBuffer);

	Optional<std::time_t> dueDate;

	if (dateStr.getSize() > 0)
	{
		try
		{
			dueDate = parseDate(dateStr);
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return;
		}
	}

	Task task(name, dueDate, Status::ON_HOLD, description);
	tasks.pushBack(task);

	int taskId = task.getId();
	// Set the owner of the task if any
	if (loggedIn)
	{
		taskToUserMap.addMapping(taskId, currentUser);
	}
	else
	{
		// This behaviour could easily be changed
		taskToUserMap.addMapping(taskId, MyString("guest"));
	}

	std::cout << "Task added successfully." << std::endl;
}

Task& TaskManager::findTask(int taskId)
{
	if (taskId < 0 || taskId >= tasks.getSize())
	{
		throw std::invalid_argument("Task does not exist");
	}

	return tasks[taskId];
}

Task& TaskManager::findTask(const MyString& name)
{
	//find the first task with id with the provided name
	for (int i = 0; i < tasks.getSize(); i++)
	{
		if (tasks[i].getName() == name)
		{
			return tasks[i];
		}
	}

	throw std::invalid_argument("Task does not exist");
}

void TaskManager::listTasksByDate(const MyString& date)
{
	std::tm tm = {};
	std::istringstream ss(date.c_str());
	ss >> std::get_time(&tm, "%Y-%m-%d");

	if (ss.fail())
	{
		std::cerr << "Invalid date format. Use YYYY-MM-DD." << std::endl;
		return;
	}

	std::time_t targetDate = std::mktime(&tm);
	if (targetDate == -1)
	{
		std::cerr << "Failed to convert date." << std::endl;
		return;
	}

	std::tm* targetTm = std::localtime(&targetDate);

	for (int i = 0; i < tasks.getSize(); i++)
	{
		Task& current = tasks[i];
		const std::time_t* dueDatePtr = current.getDueDate();

		if (dueDatePtr != nullptr)
		{
			std::tm* dueDateTm = std::localtime(dueDatePtr);

			if (dueDateTm->tm_year == targetTm->tm_year &&
				dueDateTm->tm_mon == targetTm->tm_mon &&
				dueDateTm->tm_mday == targetTm->tm_mday)
			{
				printTask(current);
				std::cout << std::endl;
			}
		}
	}
}

void TaskManager::listAllTasks()
{
	for (int i = 0; i < tasks.getSize(); i++)
	{
		// TODO: extract into a function for printing a task to reuse
		Task& current = tasks[i];
		printTask(current);
		std::cout << std::endl;
	}
}

// Possibly not needed
//Vector<Task> TaskManager::getUserTasks(const MyString& username)
//{
//	Vector<Task> currentUserTasks;
//
//	taskToUserMap.getTasksForUser(username);
//
//	return currentUserTasks;
//}

void TaskManager::handleUpdateTaskName(std::istream& is)
{
	int  id;
	MyString newName;

	is >> id;
	is >> newName;

	try
	{
		Task& taskToChange = findTask(id);
		taskToChange.setName(newName);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

}

void TaskManager::handleStartTask(std::istream& is)
{
	int  id;
	is >> id;

	try
	{
		Task& taskToChange = findTask(id);
		taskToChange.setStatus(Status::IN_PROCESS);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
}

void TaskManager::handleUpdateTaskDescription(std::istream& is)
{
	int id;
	MyString description;

	is >> id;
	is >> description;

	try
	{
		Task& taskToChange = findTask(id);
		taskToChange.setDescription(description);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
}

// TODO: extract in class like utils 
bool isNumber(const char* s)
{
	while (*s)
	{
		if (!std::isdigit(*s))
		{
			return false;
		}

		s++;
	}
	return true;
}

void TaskManager::handleGetTask(std::istream& is)
{
	MyString username;
	MyString argument;

	is >> username;
	is >> argument;

	if (isNumber(argument.c_str()))
	{
		int id = std::atoi(argument.c_str());
		Task& task = findTask(id);
		printTask(task);
	}
	else
	{
		MyString& name = argument;
		Task& task = findTask(name);
		printTask(task);
	}
}

bool isDate(const char* s)
{
	// Simple check: we assume the date is in the format "YYYY-MM-DD"
	if (strlen(s) != 10)
	{
		return false;
	}

	if (s[4] != '-' || s[7] != '-')
	{
		return false;
	}

	for (int i = 0; i < 10; ++i)
	{
		if (i == 4 || i == 7)
		{
			continue;
		}

		if (!isdigit(s[i]))
		{
			return false;
		}
	}

	return true;
}

void TaskManager::handleListTasks(std::istream& is)
{
	MyString argument;

	if (is >> argument)
	{
		if (isDate(argument.c_str()))
		{
			// Argument is a date
			listTasksByDate(argument);
		}
		else
		{
			std::cerr << "Invalid date format. Expected format: YYYY-MM-DD" << std::endl;
		}
	}
	else
	{
		// No argument provided
		listAllTasks();
	}
}

void TaskManager::handleFinishTask(std::istream& is)
{
	int id;
	is >> id;

	try
	{
		Task& taskToChange = findTask(id);
		taskToChange.setStatus(Status::DONE);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
}


void printDueDate(const std::time_t* dueDate)
{
	if (dueDate == nullptr)
	{
		std::cout << "Due date not set." << std::endl;
		return;
	}

	char buffer[100];
	std::tm* timeInfo;

	// Convert time_t to tm structure
	timeInfo = std::localtime(dueDate);

	// Format the date and time into the buffer
	std::strftime(buffer, sizeof(buffer), "Due date : %a %b %d %H : %M : %S %Y", timeInfo);

	// Print the formatted date and time
	std::cout << buffer << std::endl;
}

const char* statusToString(Status status)
{
	switch (status)
	{
	case Status::ON_HOLD: return "ON_HOLD";
	case Status::IN_PROCESS: return "IN_PROCESS";
	case Status::DONE: return "DONE";
	case Status::OVERDUE: return "OVERDUE";
	default: return "UNKNOWN";
	}
}


void printTask(const Task& task)
{
	std::cout << "Task name : " << task.getName() << std::endl;
	std::cout << "Task ID : " << task.getId() << std::endl;
	printDueDate(task.getDueDate());
	std::cout << "Task desc : " << task.getDescription() << std::endl;
	std::cout << "Status : " << statusToString(task.getStatus()) << std::endl;
}

void TaskManager::handleListCompletedTasks()
{
	//  Task name : Group_project
	//	Task ID : 1287
	//	Due date : Fri Mar 15 00 : 00 : 00 2024
	//	Task desc : example desc
	//	Status : ON HOLD

	for (int i = 0; i < tasks.getSize(); i++)
	{
		// TODO: extract into a function for printing a task to reuse
		Task& current = tasks[i];
		if (current.getStatus() == Status::DONE)
		{
			printTask(current);
			std::cout << std::endl;
		}

		//std::cout << "Task name : " << current.getName() << std::endl;
		//std::cout << "Task ID : " << current.getId() << std::endl;
		//printDueDate(current.getDueDate());
		//std::cout << "Task desc : " << current.getDescription() << std::endl;
		//std::cout << "Status : " << statusToString(current.getStatus()) << std::endl;
	}
}

void TaskManager::handleDeleteTask(std::istream& is)
{
	int id;
	is >> id;

	try
	{
		// assert exists done inside popAt function
		tasks.popAt(id);
		taskToUserMap.removeMappingByTaskId(id);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
}

void TaskManager::start(std::istream& is, const char* userDataFile)
{
	// Ensure data file exists (needed for the first run)
	bool fileExists = UserSerializer::ensureDataFileCreated(userDataFile);

	if (!fileExists)
	{
		throw std::runtime_error("Data file could not be generated");
	}

	// Get initial state
	usersState = UserSerializer::readUsers(userDataFile);

	MyString firstCommand;
	is >> firstCommand;
	handleCommands(firstCommand, is, userDataFile);
}

