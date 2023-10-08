#pragma once

namespace DB
{

class DBConnections
{
public:
    ~DBConnections();

    void Connect(const char* _db, const char* _user = nullptr, const char* _password = nullptr);
    void Disconnect();


    SQLHDBC m_DBC;
    SQLHENV m_ENV;
};

}