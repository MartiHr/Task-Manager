#include <fstream>

#include "UserSerializer.h"
#include "MyString.h"
#include "ExceptionMessages.h"

void UserSerializer::saveUser(const User& user, const char* fileName)
{
	std::ofstream file(fileName);

	if (!file.is_open())
	{
		throw std::runtime_error(ExceptionMessages::FILE_OPEN_WRITE_ERROR);
	}

	size_t usernameLength = user.username.getSize();
	size_t passwordLength = user.password.getSize();

	file.write(reinterpret_cast<const char*>(&usernameLength), sizeof(usernameLength));
	file.write(user.username.c_str(), usernameLength);

	file.write(reinterpret_cast<const char*>(&passwordLength), sizeof(passwordLength));
	file.write(user.password.c_str(), passwordLength);
}

User UserSerializer::readUser(const char* fileName)
{
	User user;

	std::ifstream file(fileName, std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error(ExceptionMessages::FILE_OPEN_READ_ERROR);
	}

	size_t usernameLength;
	size_t passwordLength;

	file.read(reinterpret_cast<char*>(&usernameLength), sizeof(usernameLength));
	char* tempUsername = new char [usernameLength + 1];
	file.read(tempUsername, usernameLength);
	tempUsername[usernameLength] = '\0';

	user.username = MyString(tempUsername);
	delete[] tempUsername;

	file.read(reinterpret_cast<char*>(&passwordLength), sizeof(passwordLength));
	char* tempPassword = new char[passwordLength + 1];
	file.read(tempPassword, passwordLength);
	tempPassword[passwordLength] = '\0';
	
	user.password = MyString(tempPassword);
	delete[] tempPassword;
}
