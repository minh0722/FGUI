#include "pch.h"
#include "DBRequest.h"
#include "DBTable.h"
#include "DBField.h"

#include <sstream>

namespace DB
{

DBRequest::DBRequest(u32 _tableCount/* = 0*/)
{
    m_Tables.reserve(_tableCount);
}

u32 DBRequest::Search() const
{
    // SELECT part
    std::stringstream selectStream;
    selectStream << "SELECT ";

    for (const auto& table : m_Tables)
    {
        const char* tableAlias = table.GetAlias();
        const auto& fields = table.GetFields();
        const size_t fieldsCount = fields.size();

        for (u32 i = 0; i < fieldsCount; ++i)
        {
            selectStream << tableAlias << fields[i];

            if (i != fieldsCount - 1)
            {
                selectStream << ",";
            }
        }
    }

    // FROM part
    std::stringstream fromStream;
    fromStream << " FROM ";

    const size_t tablesCount = m_Tables.size();
    for (u32 i = 0; i < tablesCount; ++i)
    {
        fromStream << m_Tables[i];

        if (i != tablesCount - 1)
        {
            fromStream << ",";
        }
    }

    // WHERE part
    size_t joinsCount = m_Joins.size();

    std::stringstream whereStream;
    if (joinsCount > 0)
    {
        whereStream << " WHERE ";
    }

    for (u32 i = 0; i < joinsCount; ++i)
    {
        u32 leftTableIdx = m_Joins[i].m_LeftTableIdx;
        u32 rightTableIdx = m_Joins[i].m_RightTableIdx;
        u32 leftFieldIdx = m_Joins[i].m_LeftFieldIdx;
        u32 rightFieldIdx = m_Joins[i].m_RightFieldIdx;
        const auto& leftTable = m_Tables[leftTableIdx];
        const auto& rightTable = m_Tables[rightTableIdx];
        const auto& leftField = m_Tables[leftTableIdx].GetFields()[leftFieldIdx];
        const auto& rightField = m_Tables[rightTableIdx].GetFields()[rightFieldIdx];

        whereStream << leftTable.GetAlias() << leftField << " = " << rightTable.GetAlias() << rightField;

        if (i != joinsCount - 1)
        {
            whereStream << " AND ";
        }
    }

    std::string sql = selectStream.str() + fromStream.str() + whereStream.str();

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
    return static_cast<u32>(m_Tables.size() - 1);
}

u32 DBRequest::AddDBTable(const DBTable& _table)
{
    m_Tables.push_back(_table);
    return static_cast<u32>(m_Tables.size() - 1);
}

u32 DBRequest::AddDBField(u32 _tableIdx, DBField&& _field)
{
    m_Tables[_tableIdx].AddDBField(std::move(_field));
    return static_cast<u32>(m_Tables.size() - 1);
}

u32 DBRequest::AddDBField(u32 _tableIdx, const DBField& _field)
{
    m_Tables[_tableIdx].AddDBField(_field);
    return static_cast<u32>(m_Tables.size() - 1);
}

u32 DBRequest::AddDBJoin(DBJoin&& _join)
{
    m_Joins.emplace_back(_join);
    return static_cast<u32>(m_Joins.size() - 1);
}

u32 DBRequest::AddDBJoin(const DBJoin& _join)
{
    m_Joins.push_back(_join);
    return static_cast<u32>(m_Joins.size() - 1);
}

}