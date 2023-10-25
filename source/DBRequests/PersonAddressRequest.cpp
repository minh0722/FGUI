#include "pch.h"

#include "PersonAddressRequest.h"
#include "PersonAddressTable.h"

namespace DB
{

PersonAddressRequest::PersonAddressRequest(const DBConnections* _conn)
    : DBRequest(_conn, 1)
{
    AddDBTable(PersonAddressTable());
}

}