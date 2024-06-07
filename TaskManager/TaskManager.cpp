#include "TaskManager.h"
#include "MyString.h"
#include "UserSerializer.h"

void TaskManager::handleCommands(MyString& command, std::istream& is, const char* userDataFile)
{
	while (command != "exit")
	{
		if (command == "register")
		{
			handleRegister(is, userDataFile);
		}
		else if (command == "login")
		{

		}
		else if(command == "add-task")
		{

		}
		else if (command == "exit")
		{
			break;
		}
		else
		{
			std::cout << "No such command";
		}

		std::cin >> command;
	}
}

void TaskManager::handleRegister(std::istream& is, const char* userDataFile)
{
	MyString username;
	MyString password;

	is >> username;
	is >> password;

	// add to binary
	User currentUser = { username, password };
	UserSerializer::saveUser(currentUser, userDataFile);
	
	// add to state
	usersState.pushBack(currentUser);
}

void TaskManager::handleLogin(std::istream& is, const char* userDataFile)
{

}

void TaskManager::start(std::istream& is, const char* userDataFile)
{
	
	//TODO: on start set users collection
	Vector<User> users = UserSerializer::readUsers(userDataFile);

	MyString firstCommand;
	is >> firstCommand;
	handleCommands(firstCommand, is, userDataFile);
}