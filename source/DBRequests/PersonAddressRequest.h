#pragma once
#include "DBRequest.h"

class DBConnections;

namespace DB
{

class PersonAddressRequest : public DBRequest
{
public:
    PersonAddressRequest(const DBConnections* _conn);
};

}