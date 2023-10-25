#pragma once

#include <vector>
#include <iostream>

#include <d3d11.h>

#include <tchar.h>
#include <wrl/client.h>
#include <sql.h>
#include <sqlext.h>

#include "CTypes.h"

#define SQL_THROW_IF_FAIL(ret)                      \
    if(!SQL_SUCCEEDED(ret))                         \
    {                                               \
        char buf[20];                               \
        snprintf(buf, 20, "SQL error: %d", ret);    \
        printf("%s", buf);                          \
        throw std::exception();                     \
    }