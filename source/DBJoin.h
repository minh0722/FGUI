#pragma once

namespace DB
{

enum class JoinType
{
    Inner,
    Left,
    Right,
    Full
};

struct DBJoin
{
    DBJoin(i32 _leftTableIdx, i32 _rightTableIdx, i32 _leftFieldIdx, i32 _rightFieldIdx = -1);

    i32 m_LeftTableIdx = 0;
    i32 m_RightTableIdx = 0;
    i32 m_LeftFieldIdx = 0;
    i32 m_RightFieldIdx = 0;
    JoinType m_JoinType = JoinType::Inner;
};

}