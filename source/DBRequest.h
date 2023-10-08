#pragma once

#include "DBTable.h"
#include "DBJoin.h"

namespace DB
{

class DBConnections;

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
    DBRequest(const DBConnections* _conn, u32 _tableCount = 0);
    ~DBRequest();

    u32 Search() const;
    void Insert();
    void Update();

    template <typename T>
    T GetSearchValue(u32 _rowIdx, u32 _columnIdx);

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
    const DBConnections* m_Conn;
    
    mutable SQLHSTMT m_Stmt = nullptr;
};

}