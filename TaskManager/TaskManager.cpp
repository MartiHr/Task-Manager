#include "TaskManager.h"
#include "MyString.h"

void TaskManager::handleCommands(MyString& command, std::istream& is)
{
	while (command != "exit")
	{
		if (command == "register")
		{
			handleRegister(is);
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

void TaskManager::handleRegister(std::istream& is)
{
	MyString username;
	MyString password;

	is >> username;
	is >> password;


}

void TaskManager::start(std::istream& is)
{
	MyString firstCommand;
	is >> firstCommand;
	handleCommands(firstCommand, is);

}
