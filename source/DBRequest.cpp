#include "pch.h"
#include "DBRequest.h"

DBRequest::DBRequest(u32 _columnCount/* = 0*/)
{
    m_Columns.reserve(_columnCount);
}

size_t DBRequest::AddDBColumn(DBColumn&& _field)
{
    m_Columns.emplace_back(std::move(_field));
    return m_Columns.size();
}

size_t DBRequest::AddDBColumn(const DBColumn& _field)
{
    m_Columns.push_back(_field);
    return m_Columns.size();
}