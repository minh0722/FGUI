#include "pch.h"
#include "DBColumn.h"

namespace DB
{

DBColumn::DBColumn(const char* _name, SQLSMALLINT _columnType)
    : m_Name(_name),
    m_ColumnType(_columnType)
{
}

const char* DBColumn::GetName() const
{
    return m_Name;
}

SQLSMALLINT DBColumn::GetColumnType() const
{
    return m_ColumnType;
}

}