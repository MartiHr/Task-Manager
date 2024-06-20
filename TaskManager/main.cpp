#pragma warning(disable:4996)
#include <iostream>
#include "User.h"
#include "UserSerializer.h"
#include "TaskManager.h"
#include "Dashboard.h"

int main()
{
	/*User u = { "george", "123123" };
	User u2 = { "2", "2" };

	UserSerializer::saveUser(u, "user.bin");

	User newU;
	UserSerializer::readUser("user.bin");
	newU = UserSerializer::readUser("user.bin");

	std::cout << newU.username << newU.password;*/

	//TaskManager::start(std::cin, "users.bin");

	/*MyString str;
	std::cin >> str;*/

	//Optional<std::time_t> time = 

	/*Vector<int> arr;
	arr.pushBack(1);
	arr.pushBack(2);
	arr.pushBack(3);
	arr.pushBack(4);
	arr.pushBack(5);

	for (int i = 0; i < arr.getSize(); i++)
	{
		if (i == 3)
		{
			arr.popAt(i);
		}
	}

	for (int i = 0; i < arr.getSize(); i++)
	{
		std::cout << arr[i] << std::endl;
	}*/

	TaskManager::start(std::cin, "users.bin");
}