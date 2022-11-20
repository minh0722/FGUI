#pragma once

#include "DBField.h"

namespace DB
{

class DBTable
{
public:
    DBTable(const char* _name, const char* _alias, const char* _schema = "dbo");

    void AddDBField(const DBField& _field);
    void AddDBField(DBField&& _field);

    const std::vector<DBField>& GetFields() const;
    const char* GetAlias() const;

    friend std::ostream& operator<<(std::ostream& _os, const DBTable& _table);

private:
    const char* m_Name = nullptr;
    const char* m_Alias = nullptr;
    const char* m_Schema = nullptr;

    std::vector<DBField> m_Fields;
};

}