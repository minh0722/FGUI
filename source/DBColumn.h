#pragma once

class DBColumn
{
public:
    DBColumn(const char* _name, SQLSMALLINT _columnType);

    const char* GetName() const;
    SQLSMALLINT GetColumnType() const;

private:
    const char* m_Name = nullptr;
    SQLSMALLINT m_ColumnType = -1;
};
