#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "TaskManager.h"
#include "MyString.h"
#include "UserSerializer.h"

Vector<User> TaskManager::usersState;
Vector<Task> TaskManager::tasks;
Dashboard TaskManager::dashboard;

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

		}
		else if (command == "list-tasks")
		{

		}
		else if (command == "list-completed-tasks")
		{

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
	MyString description(descriptionBuffer);

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

	tasks.pushBack(Task(name, dueDate, Status::ON_HOLD, description));
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
	int  id;
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

void TaskManager::handleDeleteTask(std::istream& is)
{
	int id;
	is >> id;

	try
	{
		// assert exists done inside popAt function
		tasks.popAt(id);
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

