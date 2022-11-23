#pragma once
#include "DBTable.h"

namespace DB
{

class PersonAddressTable : public DBTable
{
public:
    enum Field
    {
        AddressID,
        AddressLine1,
        AddressLine2,
        City,
        StateProvinceId,
        PostalCode,
        SpatialLocation,
        rowguid,
        ModifiedDate,
        FieldCount
    };

    PersonAddressTable();
};

}