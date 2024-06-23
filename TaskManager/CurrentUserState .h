#pragma once
#include "MyString.h"

class CurrentUserState
{
public:
    //TODO: change behaviour (add get and set functions) or make a struct
    static bool loggedIn;
    static int currentDashboard;
    static MyString currentUser;
};