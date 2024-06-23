#include "DashboardCollection.h"

void DashboardCollection::addDashboard(const Dashboard& dashboard)
{
	dashboards.pushBack(dashboard);
}

void DashboardCollection::removeDashboard(int index)
{
	if (index < dashboards.getSize())
	{
		dashboards.popAt(index);
	}
}

Dashboard& DashboardCollection::getDashboard(int index)
{
	return dashboards[index];
}

int DashboardCollection::getCount() const
{
	return dashboards.getSize();
}

Dashboard& DashboardCollection::findDashboardByName(const MyString& username)
{
    for (int i = 0; i < dashboards.getSize(); ++i)
    {
        if (dashboards[i].getOwnerUsername() == username)
        {
            return dashboards[i];;
        }
    }

    throw std::exception("No dashboard with this owner exists");
}

void DashboardCollection::setDashboards(const Vector<Dashboard>& newDashboards)
{
    dashboards = newDashboards;
}

void DashboardCollection::ensureExists(const MyString& owner)
{
	try
	{
		findDashboardByName(owner);
	}
	catch (const std::exception&)
	{
		Dashboard currentUserDashboard(owner);
		addDashboard(currentUserDashboard);
	}
}
