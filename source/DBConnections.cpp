#include "pch.h"
#include "DBConnections.h"
#include <sstream>
#include <memory>

namespace DB
{

DBConnections::~DBConnections()
{
    SQLFreeHandle(SQL_HANDLE_DBC, m_DBC);
    SQLFreeHandle(SQL_HANDLE_ENV, m_ENV);
}

void DBConnections::Connect(const char* _db, const char* _user /*= nullptr*/, const char* _password /*= nullptr*/)
{
    SQL_THROW_IF_FAIL(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_ENV));
    SQL_THROW_IF_FAIL(SQLSetEnvAttr(m_ENV, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0));

    u16 length = strlen(_db) + 10;
    std::unique_ptr<char[]> dsnString = std::make_unique<char[]>(length);
    snprintf(dsnString.get(), length, "DSN=%s;", _db);

    SQL_THROW_IF_FAIL(SQLAllocHandle(SQL_HANDLE_DBC, m_ENV, &m_DBC));

    auto ret = (SQLDriverConnect(m_DBC,
        NULL,
        reinterpret_cast<SQLCHAR*>(dsnString.get()),
        strlen((char*)dsnString.get()),
        NULL,
        0,
        NULL,
        SQL_DRIVER_NOPROMPT));

    if (ret == SQL_ERROR)
    {
        int rec = 1;
        SQLSTATE state;
        SQLCHAR errorMsg[500];

        SQLGetDiagRec(SQL_HANDLE_DBC, m_DBC, rec, state, NULL, errorMsg, 500, NULL);
        std::cout << "Error code: " << state << std::endl << "Error msg: " << errorMsg << std::endl;

        SQL_THROW_IF_FAIL(ret);
    }
}

void DBConnections::Disconnect()
{
    SQLDisconnect(m_DBC);
}

}