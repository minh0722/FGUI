#include "pch.h"
#include "DBRequest.h"

namespace DB
{

DBRequest::DBRequest(u32 _columnCount/* = 0*/)
{
    m_Tables.reserve(_columnCount);
}

u32 DBRequest::Search()
{
    return 0;
}

void DBRequest::Insert()
{

}

void DBRequest::Update()
{

}

u32 DBRequest::AddDBTable(DBTable&& _table)
{
    m_Tables.emplace_back(_table);
    return m_Tables.size();
}

u32 DBRequest::AddDBTable(const DBTable& _table)
{
    m_Tables.push_back(_table);
    return m_Tables.size();
}

u32 DBRequest::AddDBField(u32 _tableIdx, DBField&& _field)
{
    m_Tables[_tableIdx].AddDBField(std::move(_field));
    return m_Tables.size();
}

u32 DBRequest::AddDBField(u32 _tableIdx, const DBField& _field)
{
    m_Tables[_tableIdx].AddDBField(_field);
    return m_Tables.size();
}

}