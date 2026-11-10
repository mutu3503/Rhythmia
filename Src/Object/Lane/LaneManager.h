#pragma once
#include <DxLib.h>
#include <vector>
#include "../../CsvData/Chart/ChartData.h"
#include "../../Manager/ScoreManager.h"
#include "Lane.h"

class LaneManager
{
public:
	
	void Init(void);		// 4本のレーンを初期化
	void Update(double musicTimeSeconds, bool enableJudgment = true);	// 曲の現在時刻で全レーンを更新
	void Draw(void) const;	// レーンを描画

	// CSVの譜面を各レーンに配置
	bool SetChart(const std::vector<ChartData>& chartDataList,
		int bpm,
		double firstBeatSeconds,
		int normalImageHandle,
		int holdImageHandle
	);

	// 最後の通常ノーツまたはロングノーツの終了時刻を取得する
	double GetChartEndSeconds(void) const { return chartEndSeconds_; }

	// プレイの集計結果をコピー
	PlayResult GetPlayResult(void) const
	{
		return scoreManager_.GetResult();
	}

private:

	// この譜面のスコアと判定定数を管理
	ScoreManager scoreManager_;

	// レーン数
	static constexpr int LANE_COUNT = 4;

	// レーン全体の配置（ピクセル）
	static constexpr int LANE_LEFT = 400;
	static constexpr int LANE_TOP = 120;
	static constexpr int LANE_WIDTH = 120;
	static constexpr int LANE_BOTTOM = 650;

	// 判定ラインのY座標（ピクセル）
	static constexpr int JUDGE_LINE_Y = 580;

	// BPMから秒に変換するための単位定数
	static constexpr double SECONDS_PER_MINUTE = 60.0;

	// 曲と拍の開始位置
	static constexpr double MUSIC_START_SECONDS = 0.0;
	static constexpr double FIRST_BEAT = 0.0;

	// 有効なレーン番号の下限
	static constexpr int FIRST_LANE_IDX = 0;

	// 画像ハンドルの無効値
	static constexpr int INVALID_IMAGE_HANDLE = -1;

	// 1フレーム分の判定を受け取る
	std::vector<JUDGMENT> finishedJudgments_;

	// BPMとタイミング補正を適応したあとの譜面終了時刻
	double chartEndSeconds_ = MUSIC_START_SECONDS;

	// 担当キー表示名
	const char* keyNames_[LANE_COUNT] = { "D","F","J","K" };

	// 各レーンで使用するキー
	const int laneKeys_[LANE_COUNT] =
	{
		KEY_INPUT_D,
		KEY_INPUT_F,
		KEY_INPUT_J,
		KEY_INPUT_K,
	};

	Lane lanes_[LANE_COUNT];
};

