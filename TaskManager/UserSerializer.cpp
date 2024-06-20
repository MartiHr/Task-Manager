#include <fstream>

#include "UserSerializer.h"
#include "MyString.h"
#include "ExceptionMessages.h"

void UserSerializer::saveUser(const User& user, const char* fileName)
{
	std::ofstream file(fileName, std::ios::binary | std::ios::app);

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

	file.close();
}

User UserSerializer::readUser(std::ifstream& file)
{
	User user;

	size_t usernameLength;
	size_t passwordLength;

	// Read username
	file.read(reinterpret_cast<char*>(&usernameLength), sizeof(usernameLength));
	char* tempUsername = new char[usernameLength + 1];
	file.read(tempUsername, usernameLength);
	tempUsername[usernameLength] = '\0';

	user.username = MyString(tempUsername);
	delete[] tempUsername;

	// Read password
	file.read(reinterpret_cast<char*>(&passwordLength), sizeof(passwordLength));
	char* tempPassword = new char[passwordLength + 1];
	file.read(tempPassword, passwordLength);
	tempPassword[passwordLength] = '\0';

	user.password = MyString(tempPassword);
	delete[] tempPassword;

	return user;
}

Vector<User> UserSerializer::readUsers(const char* fileName)
{
	Vector<User> users;

	std::ifstream file(fileName, std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error(ExceptionMessages::FILE_OPEN_READ_ERROR);
	}

	while (true)
	{
		try 
		{
			// Attempt to read a user
			User user = readUser(file);

			// Check if we've reached the end of the file
			if (file.eof())
			{
				break;
			}

			users.pushBack(user);
		}
		catch (const std::exception& e)
		{
			break;
		}
	}

	file.close();

	return users;
}

bool UserSerializer::ensureDataFileCreated(const char* fileName)
{
	std::ifstream file(fileName);

	if (file.good())
	{
		file.close();
		return true;
	}

	// If the file does not exist, create it
	std::ofstream createFile(fileName);

	if (!createFile)
	{
		return false;
	}

	createFile.close();

	return true;
}
