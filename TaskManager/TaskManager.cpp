#pragma warning(disable : 4996)
#include <ctime>
#include <iomanip>
#include <iostream>
#include <iostream>
#include <sstream>
#include "TaskManager.h"
#include "MyString.h"
#include "UserSerializer.h"
#include "Pair.hpp"

UserCollection TaskManager::usersState;
TaskCollection TaskManager::tasks;
TaskToUserMap TaskManager::taskToUserMap;

DashboardCollection TaskManager::dashboards;

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
			handleRemoveTaskFromDashboard(ss);
		}
		else if (command == "add-task-to-dashboard")
		{
			handleAddTaskToDashboard(ss);
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
			handleListDashboard();
		}
		else if (command == "finish-task")
		{
			// check whether additional logic is needed for the funciton
			handleFinishTask(ss);
		}
		else if (command == "exit")
		{
			// TODO: FREE MEMORY HERE!!!!! -> for Vector<T*>
			// TODO: save files (bonus)
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

	try
	{
		usersState.addUser(currentUser);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	UserSerializer::saveUser(currentUser, userDataFile);

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

	bool profileExists = usersState.checkUserExists(loginUsername, loginPassword);

	if (profileExists)
	{
		currentUserState.loggedIn = true;
		currentUserState.currentUser = loginUsername;
		
		// Ensure dashboard exists
		dashboards.ensureExists(loginUsername);
		Dashboard& currentUserDashboard = dashboards.findDashboardByName(loginUsername);
		currentUserDashboard.populateDashboard(tasks.getTaskIdsForToday());
		currentUserDashboard.depopulateDashboard(tasks.getExpiredTaskIds());

		std::cout << "Welcome back, " << loginUsername << "!" << std::endl;
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

// TODO: could be extracted to utils class
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

	char line[256]; // Assuming a reasonable max length for the description
	is.getline(line, sizeof(line));
	description = MyString(line);

	Task task(name, dueDate, Status::ON_HOLD, description);

	try
	{
		tasks.addTask(task);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	//tasks.pushBack(task);

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
	int targetYear = targetTm->tm_year;
	int targetMonth = targetTm->tm_mon;
	int targetDay = targetTm->tm_mday;

	// could be omitted if a dictionary-like class is introduced
	Vector<int> taskIds = taskToUserMap.getTasksForUser(currentUserState.currentUser);
	for (int i = 0; i < taskIds.getSize(); i++)
	{
		try
		{
			Task& current = tasks.findTask(taskIds[i]);

			const std::time_t* dueDatePtr = current.getDueDate();

			if (dueDatePtr != nullptr)
			{
				std::tm* dueDateTm = std::localtime(dueDatePtr);

				if (dueDateTm->tm_year == targetYear &&
					dueDateTm->tm_mon == targetMonth &&
					dueDateTm->tm_mday == targetDay)
				{
					printTask(current);
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void TaskManager::listAllTasks()
{
	
	// could be omitted if a dictionary-like class would be introduced
	Vector<int> taskIds = taskToUserMap.getTasksForUser(currentUserState.currentUser);
	for (int i = 0; i < taskIds.getSize(); i++)
	{
		try
		{
			int taskId = taskIds[i];
			Task& current = tasks.findTask(taskId);
			printTask(current);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
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

void TaskManager::populateDashboard(Dashboard& dashboard)
{

}

void TaskManager::handleUpdateTaskName(std::istream& is)
{
	int  id;
	MyString newName;

	is >> id;
	is >> newName;

	try
	{
		tasks.updateTaskName(id, newName);
		std::cout << "Changed the name of the task" << std::endl;
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
		tasks.startTask(id);

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
	char line[256]; // Assuming a reasonable max length for the description
	is.getline(line, sizeof(line));
	description = MyString(line);

	try
	{
		tasks.updateTaskDescription(id, description);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::cout << "Task description updated" << std::endl;
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
			Task& task = tasks.findTask(id);
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
			Task& task = tasks.findTask(name);
			printTask(task);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return;
		}

	}
}

void TaskManager::handleListTasks(std::stringstream& ss)
{
	if (!currentUserState.loggedIn)
	{
		std::cout << "You should login first" << std::endl;
		return;
	}

	char argument[256]; // Assuming a reasonable max length for the argument

	if (ss >> argument)
	{
		try
		{
			// Try parsing the date
			parseDate(MyString(argument));
			// Argument is a date
			listTasksByDate(MyString(argument));
		}
		catch (const std::exception&)
		{
			std::cerr << "Invalid date format. Expected format: YYYY-MM-DD" << std::endl;
		}
	}
	else
	{
		// No argument provided, list all tasks
		listAllTasks();
	}
}

void TaskManager::handleFinishTask(std::istream& is)
{
	int id;
	is >> id;

	try
	{
		tasks.finishTask(id);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
}

void TaskManager::handleListCompletedTasks()
{
	if (!currentUserState.loggedIn)
	{
		std::cout << "You should login first" << std::endl;
		return;
	}

	// could be omitted if a dictionary-like class would be introduced
	Vector<int> taskIds = taskToUserMap.getTasksForUser(currentUserState.currentUser);
	for (int i = 0; i < taskIds.getSize(); i++)
	{
		try
		{
			Task& current = tasks.findTask(taskIds[i]);

			if (current.getStatus() == Status::DONE)
			{
				printTask(current);
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void TaskManager::handleDeleteTask(std::istream& is)
{
	int id;
	is >> id;

	try
	{
		tasks.removeTask(id);
		taskToUserMap.removeMappingByTaskId(id);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::cout << "Successfully deleted task with id: " << id << std::endl;
}

void TaskManager::handleAddTaskToDashboard(std::istream& is)
{
	if (!currentUserState.loggedIn)
	{
		std::cout << "You should login first" << std::endl;
		return;
	}

	int id;
	is >> id;

	try
	{
		Dashboard& currentUserDashboard = dashboards.findDashboardByName(currentUserState.currentUser);
		Task& currentTask = tasks.findTask(id);
		
		if (currentTask.getStatus() != Status::OVERDUE)
		{
			currentUserDashboard.addTask(id);
			std::cout << "Task successfully added to dashboard" << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
}

void TaskManager::handleRemoveTaskFromDashboard(std::istream& is)
{
	if (!currentUserState.loggedIn)
	{
		std::cout << "You should login first" << std::endl;
		return;
	}

	int id;
	is >> id;

	try
	{
		Dashboard& currentUserDashboard = dashboards.findDashboardByName(currentUserState.currentUser);
		// To validate the task and for extensibility
		Task& currentTask = tasks.findTask(id);

		currentUserDashboard.removeTask(id);
		std::cout << "Task successfully removed from dashboard" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
}

void TaskManager::handleListDashboard()
{
	if (!currentUserState.loggedIn)
	{
		std::cout << "You should login first" << std::endl;
		return;
	}

	try
	{
		Dashboard& currentUserDashboard = dashboards.findDashboardByName(currentUserState.currentUser);
		Vector<int> dashboardIds = currentUserDashboard.getTaskIds();

		int size = dashboardIds.getSize();
		for (int i = 0; i < size; i++)
		{
			int currentTaskId = dashboardIds[i];
			printTask(tasks.findTask(currentTaskId));
		}
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
	usersState.setUsers(UserSerializer::readUsers(userDataFile));

	// BONUS set tasks state, the the map state and dashboard state 
	// (not stated in the problem description). Would be an easy addition.

	handleCommands(is, userDataFile);
}

