#include "pch.h"
#include "DBTable.h"

namespace DB
{

DBTable::DBTable(const char* _name, const char* _alias, const char* _schema /*= "dbo"*/)
    : m_Name(_name),
    m_Alias(_alias),
    m_Schema(_schema)
{
}

void DBTable::AddDBField(const char* _name, SQLSMALLINT _columnType)
{
    m_Fields.emplace_back(DBField(_name, _columnType));
}

void DBTable::AddDBField(const DBField& _field)
{
    m_Fields.push_back(_field);
}

void DBTable::AddDBField(DBField&& _field)
{
    m_Fields.emplace_back(std::move(_field));
}

const std::vector<DBField>& DBTable::GetFields() const
{
    return m_Fields;
}

const char* DBTable::GetAlias() const
{
    return m_Alias;
}

std::ostream& operator<<(std::ostream& _os, const DBTable& _table)
{
    _os << _table.m_Schema << "." << _table.m_Name << " " << _table.m_Alias;

    return _os;
}

}