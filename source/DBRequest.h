#pragma once

#include "DBColumn.h"


class DBRequest
{
public:
    DBRequest(u32 _columnCount = 0);

protected:
    size_t AddDBColumn(DBColumn&& _field);
    size_t AddDBColumn(const DBColumn& _field);


protected:
    std::vector<DBColumn> m_Columns;
};

