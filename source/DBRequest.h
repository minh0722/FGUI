#pragma once

#include "DBColumn.h"


class DBRequest
{
public:
    DBRequest(u32 _columnCount = 0);

    u32 Search();
    void Insert();
    void Update();

protected:
    size_t AddDBColumn(DBColumn&& _field);
    size_t AddDBColumn(const DBColumn& _field);


protected:
    std::vector<DBColumn> m_Columns;
};

