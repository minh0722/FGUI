#include "pch.h"
#include "DBJoin.h"

namespace DB
{

DBJoin::DBJoin(i32 _leftTableIdx, i32 _rightTableIdx, i32 _leftFieldIdx, i32 _rightFieldIdx /*= -1*/)
    : m_LeftTableIdx(_leftTableIdx),
    m_RightTableIdx(_rightTableIdx),
    m_LeftFieldIdx(_leftFieldIdx),
    m_RightFieldIdx(_rightFieldIdx)
{
    if (m_RightFieldIdx == -1)
    {
        m_RightFieldIdx = m_LeftFieldIdx;
    }
}

}