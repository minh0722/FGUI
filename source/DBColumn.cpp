#include "pch.h"
#include "DBColumn.h"

DBColumn::DBColumn(const char* _name, SQLSMALLINT _columnType)
    : m_Name(_name),
    m_ColumnType(_columnType)
{
}