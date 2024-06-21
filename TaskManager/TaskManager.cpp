#pragma warning(disable : 4996)
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
Vector<Dashboard> TaskManager::dashboards;
TaskToUserMap TaskManager::taskToUserMap;

//bool TaskManager::loggedIn = false;
CurrentUserState TaskManager::currentUserState;

void TaskManager::handleCommands(std::istream& is, const char* userDataFile)
{
	MyString command;

	// TODO: extract strings elsewhere
	while (true)
	{
		std::cout << '>';
		//is >> command;

		char line[256]; // Assuming a reasonable max length for the input line
		is.getline(line, sizeof(line));
		std::stringstream ss(line);
		ss >> command;
		
		if (command == "register")
		{
			handleRegister(ss, userDataFile);
		}
		else if (command == "login")
		{
			handleLogin(ss);
		}
		else if (command == "logout")
		{
			handleLogout();
		}
		else if (command == "add-task")
		{
			handleAddTask(ss);
		}
		else if (command == "update-task-name")
		{
			handleUpdateTaskName(ss);
		}
		else if (command == "start-task")
		{
			handleStartTask(ss);
		}
		else if (command == "update-task-description")
		{
			handleUpdateTaskDescription(ss);
		}
		else if (command == "remove-task-from-dashboard")
		{

		}
		else if (command == "add-task-to-dashboard")
		{

		}
		else if (command == "delete-task")
		{
			handleDeleteTask(ss);
		}
		else if (command == "get-task")
		{
			handleGetTask(ss);
		}
		else if (command == "list-tasks")
		{
			handleListTasks(ss);
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
			handleFinishTask(ss);
		}
		else if (command == "exit")
		{
			break;
		}
		else
		{
			std::cout << "No such command." << std::endl;
		}
	}
}

void TaskManager::handleRegister(std::istream& is, const char* userDataFile)
{
	if (currentUserState.loggedIn)
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
	if (currentUserState.loggedIn)
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
			currentUserState.loggedIn = true;

			// load the dashboard of the user
			//dashboard.setTasks(loginUsername);
			//dashboard.setTasks()
			break;
		}
	}

	if (!currentUserState.loggedIn)
	{
		std::cout << "Wrong credentials." << std::endl;

	}
}

void TaskManager::handleLogout()
{
	currentUserState.loggedIn = false;
	//TODO: vector may not work as intended here !!!
	//dashboard.free();
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
	MyString name, dateStr, description;
	is >> name;
	is >> dateStr;
	
	//is.ignore(); // Ignore any remaining newline character
	// Clear the newline character left in the input buffer by previous >> operations
	/*is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	char descriptionBuffer[1024];
	is.getline(descriptionBuffer, 1024);
	description = MyString(descriptionBuffer);*/

	Optional<std::time_t> dueDate;

	//if (description.getSize() > 0)
	//{
	//	try
	//	{
	//		dueDate = parseDate(dateStr);
	//	}
	//	catch (const std::runtime_error& e)
	//	{
	//		std::cerr << e.what() << std::endl;
	//		return;
	//	}
	//}
	//else
	//{
	//	description = dateStr;
	//}

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

	is >> description;

	Task task(name, dueDate, Status::ON_HOLD, description);
	tasks.pushBack(task);

	int taskId = task.getId();
	// Set the owner of the task if any
	if (currentUserState.loggedIn)
	{
		taskToUserMap.addMapping(taskId, currentUserState.currentUser);
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
		Task& current = tasks[i];
		printTask(current);
		std::cout << std::endl;
	}
}

void TaskManager::printDueDate(const std::time_t* dueDate)
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

const char* TaskManager::statusToString(Status status)
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

void TaskManager::printTask(const Task& task)
{
	std::cout << "Task name : " << task.getName() << std::endl;
	std::cout << "Task ID : " << task.getId() << std::endl;
	printDueDate(task.getDueDate());
	std::cout << "Task desc : " << task.getDescription() << std::endl;
	std::cout << "Status : " << statusToString(task.getStatus()) << std::endl;
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
		std::cout << "Changed the name of the task";
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
		std::cout << "Task started" << std::endl;
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
	MyString argument;
	is >> argument;

	if (isNumber(argument.c_str()))
	{
		int id = std::atoi(argument.c_str());

		try
		{
			Task& task = findTask(id);
			printTask(task);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return;
		}
	}
	else
	{
		MyString& name = argument;

		try
		{
			Task& task = findTask(name);
			printTask(task);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return;
		}
		
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

void TaskManager::handleListTasks(std::stringstream& ss)
{
	char argument[256]; // Assuming a reasonable max length for the argument

	if (ss >> argument)
	{
		if (isDate(argument))
		{
			// Argument is a date
			listTasksByDate(MyString(argument));
		}
		else
		{
			std::cerr << "Invalid date format. Expected format: YYYY-MM-DD" << std::endl;
		}
	}
	else
	{
		// No argument provided, list all tasks
		listAllTasks();
	}

	//if (is >> argument)
	//{
	//	if (isDate(argument))
	//	{
	//		// Argument is a date
	//		listTasksByDate(MyString(argument));
	//	}
	//	else
	//	{
	//		std::cerr << "Invalid date format. Expected format: YYYY-MM-DD" << std::endl;
	//	}
	//}
	//else
	//{
	//	// No argument provided
	//	listAllTasks();
	//}
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

//
//void printDueDate(const std::time_t* dueDate)
//{
//	if (dueDate == nullptr)
//	{
//		std::cout << "Due date not set." << std::endl;
//		return;
//	}
//
//	char buffer[100];
//	std::tm* timeInfo;
//
//	// Convert time_t to tm structure
//	timeInfo = std::localtime(dueDate);
//
//	// Format the date and time into the buffer
//	std::strftime(buffer, sizeof(buffer), "Due date : %a %b %d %H : %M : %S %Y", timeInfo);
//
//	// Print the formatted date and time
//	std::cout << buffer << std::endl;
//}

//void printTask(const Task& task)
//{
//	std::cout << "Task name : " << task.getName() << std::endl;
//	std::cout << "Task ID : " << task.getId() << std::endl;
//	printDueDate(task.getDueDate());
//	std::cout << "Task desc : " << task.getDescription() << std::endl;
//	std::cout << "Status : " << statusToString(task.getStatus()) << std::endl;
//}

void TaskManager::handleListCompletedTasks()
{
	//  Task name : Group_project
	//	Task ID : 1287
	//	Due date : Fri Mar 15 00 : 00 : 00 2024
	//	Task desc : example desc
	//	Status : ON HOLD

	for (int i = 0; i < tasks.getSize(); i++)
	{
		Task& current = tasks[i];
		if (current.getStatus() == Status::DONE)
		{
			printTask(current);
			std::cout << std::endl;
		}
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

	handleCommands(is, userDataFile);
}

