#include "User.h"
#include <vector>

bool Insound::User::isAuthorized(User::Type userType) const
{
    return this->type >= userType;
}

bool Insound::User::isStaff() const
{
    return this->type >= Type::Staff;
}

bool Insound::User::isAdmin() const
{
    return this->type >= Type::Admin;
}
