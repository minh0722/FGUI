#pragma once

class DBColumn
{
public:
    DBColumn(const char* _name, SQLSMALLINT _columnType);

private:
    const char* m_Name = nullptr;
    SQLSMALLINT m_ColumnType = -1;
};
