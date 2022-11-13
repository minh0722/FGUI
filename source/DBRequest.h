#pragma once

#include "DBField.h"

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
    size_t AddDBColumn(DBField&& _field);
    size_t AddDBColumn(const DBField& _field);


protected:
    std::vector<DBField> m_Columns;
};


}