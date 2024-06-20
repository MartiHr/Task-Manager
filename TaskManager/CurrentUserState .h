#pragma once
#include "MyString.h"

class CurrentUserState
{
public:
    static bool loggedIn;
    static int currentDashboard;
    static MyString currentUser;
};