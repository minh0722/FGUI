#include "pch.h"
#include "PersonAddressTable.h"

namespace DB
{

PersonAddressTable::PersonAddressTable()
    : DBTable("Address", "PA", "Person")
{
    m_Fields.resize(static_cast<u32>(Field::FieldCount));

    m_Fields[Field::AddressID] = DBField("AddressID", SQL_INTEGER);
    m_Fields[Field::AddressLine1] = DBField("AddressLine1", SQL_VARCHAR);
    m_Fields[Field::AddressLine2] = DBField("AddressLine2", SQL_VARCHAR);
    m_Fields[Field::City] = DBField("City", SQL_VARCHAR);
    m_Fields[Field::StateProvinceId] = DBField("StateProvinceId", SQL_INTEGER);
    m_Fields[Field::PostalCode] = DBField("PostalCode", SQL_VARCHAR);
    m_Fields[Field::SpatialLocation] = DBField("SpatialLocation", SQL_CHAR /*actual is geolocation*/);
    m_Fields[Field::rowguid] = DBField("rowguid", SQL_GUID /*actual is uniqueidentifier*/);
    m_Fields[Field::ModifiedDate] = DBField("ModifiedDate", SQL_DATETIME);
}

}