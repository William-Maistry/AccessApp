#ifndef SALICENSE_TYPES_H
#define SALICENSE_TYPES_H

#include <string>
#include <vector>

namespace SALicense
{
    struct LicenseData
    {
        std::string surname;
        std::string initials;
        std::string idNumber;
        std::string licenceNumber;
        std::string gender;
        std::string dateOfBirth;
        std::string issueDate;
        std::string expiryDate;

        std::vector<std::string> vehicleCodes;
    };
}

#endif