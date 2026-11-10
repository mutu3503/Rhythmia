#pragma once
#include "../Common/PlayResult.h"

class ScoreManager
{
public:

	// 再びプレイするときに前回の集計を消す
	void Reset();

	// ノーツ1個の最終判定を登録
	void AddJudgment(JUDGMENT judgment);

	// リザルトに渡せるように集計結果を返す
	PlayResult GetResult(void) const;

private:

	// 判定ごとの配点
	static constexpr int PERFECT_POINTS = 100;
	static constexpr int GREAT_POINTS = 70;
	static constexpr int GOOD_POINTS = 30;
	static constexpr int MISS_POINTS = 0;

	// プレイ時の集計結果
	PlayResult result_{};
};

