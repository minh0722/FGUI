#pragma once

#include "DBField.h"

namespace DB
{

class DBTable
{
public:
    DBTable(const char* _name);

    void AddDBField(const DBField& _field);
    void AddDBField(DBField&& _field);

private:
    const char* m_Name = nullptr;

    std::vector<DBField> m_Fields;
};

}