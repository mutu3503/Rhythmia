#include <cmath>
#include <DxLib.h>
#include "NotesBase.h"
#include "NoteSpeedPreview.h"

namespace
{
	// 設定画面の右側に置くレーンの配置
	constexpr int PREVIEW_LEFT = 750;
	constexpr int HEADING_Y = 100;
	constexpr int LANE_TOP = 150;
	constexpr int LANE_BOTTOM = 610;
	constexpr int LANE_WIDTH = 100;
	constexpr int LANE_COUNT = 4;
	constexpr int JUDGE_Y = 540;

	// 画像の端がレーンにかかるノーツも描くための余白
	constexpr int NOTE_DRAW_MARGIN = 40;

	// サンプルは4レーンを0.6秒ずつずらして流す
	constexpr double LANE_DELAY_SECONDS = 0.6;
	constexpr double REPEAT_SECONDS = LANE_COUNT * LANE_DELAY_SECONDS;
}

void NoteSpeedPreview::Init(int noteImageHandle)
{
	noteImageHandle_ = noteImageHandle;
	startedAt_ = std::chrono::steady_clock::now();
}

void NoteSpeedPreview::Draw(double pixelsPerSecond) const
{
	const auto textColor = GetColor(255, 255, 255);
	const auto laneColor = GetColor(35, 45, 70);
	const auto borderColor = GetColor(120, 140, 180);
	const auto judgeColor = GetColor(255, 220, 80);

	DrawString(PREVIEW_LEFT, HEADING_Y, "ノーツ速度プレビュー", textColor);

	const double elapsedSeconds = std::chrono::duration<double>(
		std::chrono::steady_clock::now() - startedAt_
	).count();

	for (int laneIdx = 0; laneIdx < LANE_COUNT; ++laneIdx)
	{
		const int left = PREVIEW_LEFT + laneIdx * LANE_WIDTH;
		const int right = left + LANE_WIDTH;

		// レーンの背景と枠を描く。
		DrawBox(left, LANE_TOP, right, LANE_BOTTOM, laneColor, true);
		DrawBox(left, LANE_TOP, right, LANE_BOTTOM, borderColor, false);

		if (noteImageHandle_ != INVALID_IMAGE_HANDLE && pixelsPerSecond > 0.0)
		{
			// 今レーンに見えている範囲のサンプルを描画
			const double earliestHit = elapsedSeconds
				- (LANE_BOTTOM - JUDGE_Y + NOTE_DRAW_MARGIN) / pixelsPerSecond;
			const double latestHit = elapsedSeconds
				+ (JUDGE_Y - LANE_TOP + NOTE_DRAW_MARGIN) / pixelsPerSecond;

			const double laneDelay = laneIdx * LANE_DELAY_SECONDS;
			const double firstHit = laneDelay + std::ceil(
				(earliestHit - laneDelay) / REPEAT_SECONDS
			) * REPEAT_SECONDS;

			for (double hitTime = firstHit; hitTime <= latestHit;
				hitTime += REPEAT_SECONDS)
			{
				// 本番と同じ画像・位置計算・レーン内の切り抜きを使う
				NotesBase note(hitTime, noteImageHandle_);
				note.Update(elapsedSeconds, JUDGE_Y, pixelsPerSecond);
				note.Draw(left, LANE_WIDTH, LANE_TOP, LANE_BOTTOM);
			}
		}

		// ノーツの上から判定ラインを描く。
		DrawLine(left, JUDGE_Y, right, JUDGE_Y, judgeColor);
	}
}
