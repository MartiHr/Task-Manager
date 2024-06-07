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

		std::cin >> command;
	}
}

void TaskManager::handleRegister(std::istream& is)
{
}

void TaskManager::start(std::istream& is)
{
	MyString firstCommand;
	is >> firstCommand;
	handleCommands(firstCommand, is);

}
