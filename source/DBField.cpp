#include "pch.h"
#include "DBField.h"

namespace DB
{

DBField::DBField()
    : m_Name(nullptr),
    m_ColumnType(-1)
{
}

DBField::DBField(const char* _name, SQLSMALLINT _columnType)
    : m_Name(_name),
    m_ColumnType(_columnType)
{
}

const char* DBField::GetName() const
{
    return m_Name;
}

SQLSMALLINT DBField::GetColumnType() const
{
    return m_ColumnType;
}

std::ostream& operator<<(std::ostream& _os, const DBField& _field)
{
    _os << "." << _field.m_Name;

    return _os;
}

}