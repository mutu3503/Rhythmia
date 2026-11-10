#pragma once
#include "../../CsvData/Chart/ChartData.h"
#include "../../Common/PlayResult.h"

class NotesBase
{
public:

	// 通常ノーツを作成
	NotesBase(double hitTimeSeconds, int imageHandle);

	// ロングノーツを作成
	NotesBase(double hitTimeSeconds,
		double endTimeSeconds,
		int imageHandle
	);

	// 曲の現在時刻からノーツの位置を更新
	void Update(double musicTimeSeconds, int judgeY);

	// レーン内にノーツ画像を描画
	void Draw(int laneLeft,
		int laneWidth, int laneTop,
		int laneBottom
	) const;

	// ノーツの判定時刻を取得
	double GetHitTimeSeconds(void) const { return hitTimeSeconds_; }
	double GetEndTimeSeconds(void) const { return endTimeSeconds_; }

	// ノーツの種類と長押しを開始しているかを取得
	bool IsHold(void) const { return type_ == NOTE_TYPE::HOLD_NOTE; }
	bool IsHolding(void) const { return isHolding_; }

	void BeginHold(JUDGMENT startJudgment)
	{
		startJudgment_ = startJudgment;
		isHolding_ = true; 
	}

	// このノーツで一度でもMISSになったか
	bool HasMissd(void) const { return hasMissd_; }

	// 長押しを中断したところを記録
	void MarkMiss(void)
	{
		hasMissd_ = true;
		isHolding_ = false;
	}

	// 押しなおし・再中断に応じて、現在の保持状態を切り替える
	void SetHolding(bool isHolding)
	{
		isHolding_ = isHolding;
	}

	// ロングノーツの始点で確定した判定を返す
	JUDGMENT GetStartJudgment(void) const
	{
		return startJudgment_;
	}

private:

	// ノーツの落下速度
	static constexpr double SCROLL_SPEED = 300.0;

	// 画像の表示サイズ
	static constexpr int NOTES_HEIGHT = 36;
	static constexpr int NOTES_HALF_HEIGHT = NOTES_HEIGHT / 2;
	static constexpr int SIDE_MARGIN = 8;

	// 画像ハンドルの無効値
	static constexpr int INVALID_IMAGE_HANDLE = -1;

	// ロング画像から切り出す範囲
	// 下のバー＝始点、上のバー＝終点、中央＝伸縮する帯
	static constexpr double CAP_TOP_RATIO = 0.04;
	static constexpr double CAP_BOTTOM_RATIO = 0.18;
	static constexpr double START_TOP_RATIO = 0.82;
	static constexpr double START_BOTTOM_RATIO = 0.96;
	static constexpr double BODY_TOP_RATIO = 0.40;
	static constexpr double BODY_BOTTOM_RATIO = 0.60;

	// 通常の明るさと離しているロングノーツの明るさ
	static constexpr int FULL_BRIGHTNESS = 255;
	static constexpr int MISSED_BRIGHTNESS = 110;

	// 始点を押したときの判定
	JUDGMENT startJudgment_ = JUDGMENT::MISS;

	// ノーツの種類
	NOTE_TYPE type_ = NOTE_TYPE::NORMAL_NOTE;

	// 判定ラインに到着する曲の時刻
	double hitTimeSeconds_;
	double endTimeSeconds_;

	// ノーツ画像のY座標
	double centerY_ = 0.0;
	double endY_ = 0.0;

	// 始点の入力に成功して、押し続けている状態
	bool isHolding_ = false;

	// MISSを記録済みか
	bool hasMissd_ = false;

	// 別のクラスから画像を参照
	int imageHandle_;
};