#include "UserCollection.h"

void UserCollection::setUsers(const Vector<User>& users)
{
    usersState = users;
}

void UserCollection::addUser(const User& user)
{
	usersState.pushBack(user);
}

bool UserCollection::checkUserExists(const MyString& loginUsername, const MyString& loginPassword) const
{
    for (int i = 0; i < usersState.getSize(); i++)
    {
        if (usersState[i].username == loginUsername && usersState[i].password == loginPassword)
        {
            return true;
        }
    }

    return false;
}
