#include "ScoreManager.h"

void ScoreManager::Reset()
{
    // スコアと全ての判定数を初期値に戻す
    result_ = PlayResult{};
}

void ScoreManager::AddJudgment(JUDGMENT judgment)
{
    // 確定した判定の件数を増やし、対応する点数を加える
    switch (judgment)
    {
    case JUDGMENT::PERFECT:
        ++result_.perfectCount;
        result_.score += PERFECT_POINTS;
        break;
    case JUDGMENT::GREAT:
        ++result_.greatCount;
        result_.score += GREAT_POINTS;
        break;
    case JUDGMENT::GOOD:
        ++result_.goodCount;
        result_.score += GOOD_POINTS;
        break;
    case JUDGMENT::MISS:
        ++result_.missCount;
        result_.score += MISS_POINTS;
        break;
    default:
        break;
    }
}

PlayResult ScoreManager::GetResult(void) const
{
    return result_;
}
