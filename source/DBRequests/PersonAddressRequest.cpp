#include "pch.h"

#include "PersonAddressRequest.h"
#include "PersonAddressTable.h"

namespace DB
{

PersonAddressRequest::PersonAddressRequest()
    : DBRequest(1)
{
    AddDBTable(PersonAddressTable());
}

}