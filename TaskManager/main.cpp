#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include "User.h"
#include "UserSerializer.h"
#include "TaskManager.h"
#include "Dashboard.h"
#include <sstream>

int main()
{
	TaskManager::start(std::cin, "users.bin");
}