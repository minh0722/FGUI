#pragma once

#include "DBTable.h"
#include "DBJoin.h"

namespace DB
{

class DBRequest
{
    /// <summary>
    /// Derived classes must declare same enum to enumerate indexes of tables
    /// </summary>
    enum Table
    {
        Base = -1
    };

public:
    DBRequest(u32 _tableCount = 0);

    u32 Search() const;
    void Insert();
    void Update();

protected:

    u32 AddDBTable(DBTable&& _table);
    u32 AddDBTable(const DBTable& _table);

    u32 AddDBField(u32 _tableIdx, DBField&& _field);
    u32 AddDBField(u32 _tableIdx, const DBField& _field);

    u32 AddDBJoin(DBJoin&& _join);
    u32 AddDBJoin(const DBJoin& _join);

protected:
    std::vector<DBTable> m_Tables;
    std::vector<DBJoin> m_Joins;
};

}