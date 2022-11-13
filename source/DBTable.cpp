#include "pch.h"
#include "DBTable.h"

namespace DB
{

DBTable::DBTable(const char* _name)
    : m_Name(_name)
{
}

void DBTable::AddDBField(const DBField& _field)
{
    m_Fields.push_back(_field);
}

void DBTable::AddDBField(DBField&& _field)
{
    m_Fields.emplace_back(std::move(_field));
}

}