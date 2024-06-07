#pragma once
#include "User.h"

class TaskManager
{
private:
	static 

	static void handleCommands(MyString& command, std::istream& is);
	static void handleRegister(std::istream& is);
public:
	static void start(std::istream& is);
};