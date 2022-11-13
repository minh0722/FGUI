#pragma once

#include "DBTable.h"

namespace DB
{

class DBRequest
{
public:
    DBRequest(u32 _columnCount = 0);

    u32 Search();
    void Insert();
    void Update();

protected:

    u32 AddDBTable(DBTable&& _table);
    u32 AddDBTable(const DBTable& _table);

    u32 AddDBField(u32 _tableIdx, DBField&& _field);
    u32 AddDBField(u32 _tableIdx, const DBField& _field);


protected:
    std::vector<DBTable> m_Tables;
};

}