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
