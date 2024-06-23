#pragma once
#include "Vector.hpp"
#include "Dashboard.h"

class DashboardCollection
{
private:
    Vector<Dashboard> dashboards;
public:
    // Add a new Dashboard to the collection
    void addDashboard(const Dashboard& dashboard);

    // Remove a Dashboard from the collection by index
    void removeDashboard(int index);

    // Get a Dashboard by index
    Dashboard& getDashboard(int index);

    // Get the number of Dashboards in the collection
    int getCount() const;

    // Find a Dashboard by a certain criteria (example by name)
    Dashboard& findDashboardByName(const MyString& name);

	void setDashboards(const Vector<Dashboard>& newDashboards);

    void ensureExists(const MyString& owner);
private:
};