#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "TaskManager.h"
#include "MyString.h"
#include "UserSerializer.h"

Vector<User> TaskManager::usersState;
Vector<Task> TaskManager::tasks;

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

	tasks.pushBack(Task(name, dueDate, Status::IN_PROCESS, description));
	std::cout << "Task added successfully." << std::endl;
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