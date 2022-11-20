#pragma once

namespace DB
{

class DBField
{
public:
    DBField(const char* _name, SQLSMALLINT _columnType);

    const char* GetName() const;
    SQLSMALLINT GetColumnType() const;

    friend std::ostream& operator<<(std::ostream& _os, const DBField& _field);

private:
    const char* m_Name = nullptr;
    SQLSMALLINT m_ColumnType = -1;
};

}