#include "UserCollection.h"

void UserCollection::setUsers(const Vector<User*>& users)
{
    usersState = users;
}

void UserCollection::addUser(User* user)
{
    if (checkUserExists(user->username, user->password))
    {
        throw std::exception("User already exists");
    }

    usersState.pushBack(new User(*user));
}

const Vector<User*>& UserCollection::getUsers() const
{
    return usersState;
}

bool UserCollection::checkUserExists(const MyString& loginUsername, const MyString& loginPassword) const
{
    for (int i = 0; i < usersState.getSize(); i++)
    {
        if (usersState[i]->username == loginUsername && usersState[i]->password == loginPassword)
        {
            return true;
        }
    }

    return false;
}

UserCollection::~UserCollection()
{
    for (size_t i = 0; i < usersState.getSize(); ++i) 
    {
        delete usersState[i]; // Delete each user object
    }
}