#include "pch.h"
#include "DBRequest.h"

namespace DB
{

DBRequest::DBRequest(u32 _columnCount/* = 0*/)
{
    m_Columns.reserve(_columnCount);
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

size_t DBRequest::AddDBColumn(DBField&& _field)
{
    m_Columns.emplace_back(std::move(_field));
    return m_Columns.size();
}

size_t DBRequest::AddDBColumn(const DBField& _field)
{
    m_Columns.push_back(_field);
    return m_Columns.size();
}

}