#include "User.h"
#include <vector>

const std::string Insound::User::Type::Guest = "Guest";
const std::string Insound::User::Type::User = "User";
const std::string Insound::User::Type::Staff = "Staff";
const std::string Insound::User::Type::Admin = "Admin";

// set up the user type level order from lowest to highest
static std::vector<std::string> userTypes = {
    Insound::User::Type::Guest,
    Insound::User::Type::User,
    Insound::User::Type::Staff,
    Insound::User::Type::Admin,
};

bool Insound::User::isAuthorized(const std::string &userType) const
{
    return getTypeLevel(this->type) >= getTypeLevel(userType);
}

bool Insound::User::isStaff() const
{
    return getTypeLevel(this->type) >= getTypeLevel(User::Type::Staff);
}

bool Insound::User::isAdmin() const
{
    return getTypeLevel(this->type) >= getTypeLevel(User::Type::Admin);
}

int Insound::User::getTypeLevel(const std::string &type)
{
    for (int i = 0; i < userTypes.size(); ++i)
        if (userTypes[i] == type) return i;

    throw std::runtime_error( f("Insound::User::getTypeLevel: invalid user "
        "type: {}", type) );
}

