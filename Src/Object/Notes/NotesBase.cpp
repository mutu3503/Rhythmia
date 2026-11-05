#include <algorithm>
#include <cmath>
#include <DxLib.h>
#include "NotesBase.h"

NotesBase::NotesBase(double hitTimeSeconds, int imageHandle)
	:
	hitTimeSeconds_(hitTimeSeconds),
	endTimeSeconds_(hitTimeSeconds),
	imageHandle_(imageHandle)
{
}

NotesBase::NotesBase(double hitTimeSeconds, double endTimeSeconds,
	int imageHandle
)
	:
	type_(NOTE_TYPE::HOLD_NOTE),
	hitTimeSeconds_(hitTimeSeconds),
	endTimeSeconds_(endTimeSeconds),
	imageHandle_(imageHandle)
{
}

void NotesBase::Update(double musicTimeSeconds, int judgeY)
{
	// 始点は、開始時刻に判定ラインへ到着する
	centerY_ = judgeY
		- (hitTimeSeconds_ - musicTimeSeconds) * SCROLL_SPEED;

	// 終点は、終了時刻に判定ラインへ到着する
	endY_ = judgeY
		- (endTimeSeconds_ - musicTimeSeconds) * SCROLL_SPEED;

	if (isHolding_ || (IsHold() && hasMissd_))
	{
		// 押し始めたら始点を判定ラインに固定する
		centerY_ = judgeY;

		// 終点が判定ラインを越えないようにする
		if (endY_ > judgeY)
		{
			endY_ = judgeY;
		}
	}
}

void NotesBase::Draw(int laneLeft, int laneWidth, int laneTop,
	int laneBottom) const
{
	if (imageHandle_ == INVALID_IMAGE_HANDLE)
	{
		return;
	}

	// ロングは始点から終点までの全体で画面内かを確認する
	const double upperY = IsHold()
		? (std::min)(centerY_, endY_) : centerY_;
	const double lowerY = IsHold()
		? (std::max)(centerY_, endY_) : centerY_;

	if (lowerY + NOTES_HALF_HEIGHT < laneTop
		|| upperY - NOTES_HALF_HEIGHT > laneBottom)
	{
		return;
	}

	// 呼び出し前の描画範囲を保存する
	RECT oldArea{};
	GetDrawArea(&oldArea);

	// RECTの座標を、レーンと同じint型にそろえる
	const int oldLeft = static_cast<int>(oldArea.left);
	const int oldTop = static_cast<int>(oldArea.top);
	const int oldRight = static_cast<int>(oldArea.right);
	const int oldBottom = static_cast<int>(oldArea.bottom);

	// 元の描画範囲とレーンが重なる部分を求める
	const int laneRight = laneLeft + laneWidth;
	const int clipLeft = (std::max)(oldLeft, laneLeft);
	const int clipTop = (std::max)(oldTop, laneTop);
	const int clipRight = (std::min)(oldRight, laneRight);
	const int clipBottom = (std::min)(oldBottom, laneBottom);

	if (clipLeft >= clipRight || clipTop >= clipBottom)
	{
		return;
	}

	const int left = laneLeft + SIDE_MARGIN;
	const int right = laneRight - SIDE_MARGIN;
	const int startY = static_cast<int>(std::lround(centerY_));
	const int finishY = static_cast<int>(std::lround(endY_));

	int imageWidth = 0;
	int imageHeight = 0;

	// ロングノーツ画像の切り出しに使う元画像サイズを取得する
	if (IsHold())
	{
		GetGraphSize(imageHandle_, &imageWidth, &imageHeight);

		if (imageWidth <= 0 || imageHeight <= 0)
		{
			return;
		}
	}

	SetDrawArea(clipLeft, clipTop, clipRight, clipBottom);

	// 呼び出し前の明るさを保存する
	int oldRed;
	int oldGreen;
	int oldBlue;

	GetDrawBright(&oldRed, &oldGreen, &oldBlue);

	// 離してるロングノーツだけを暗くする
	if (IsHold() && hasMissd_ && !isHolding_)
	{
		SetDrawBright(
			oldRed * MISSED_BRIGHTNESS / FULL_BRIGHTNESS,
			oldGreen * MISSED_BRIGHTNESS / FULL_BRIGHTNESS,
			oldBlue * MISSED_BRIGHTNESS / FULL_BRIGHTNESS
		);
	}

	if (!IsHold())
	{
		// 通常ノーツは画像全体を描画する
		DrawExtendGraph(
			left, startY - NOTES_HALF_HEIGHT,
			right, startY + NOTES_HALF_HEIGHT,
			imageHandle_, true
		);
	}
	else
	{
		// 切り出し位置を、画像の比率からピクセルへ変換する
		const int bodyTop =
			static_cast<int>(imageHeight * BODY_TOP_RATIO);
		const int bodyBottom =
			static_cast<int>(imageHeight * BODY_BOTTOM_RATIO);

		const int capTop =
			static_cast<int>(imageHeight * CAP_TOP_RATIO);
		const int capBottom =
			static_cast<int>(imageHeight * CAP_BOTTOM_RATIO);

		const int startTop =
			static_cast<int>(imageHeight * START_TOP_RATIO);
		const int startBottom =
			static_cast<int>(imageHeight * START_BOTTOM_RATIO);

		// 帯を先に描画し、上下のバーでつなぎ目を隠す
		if (finishY < startY)
		{
			DrawRectExtendGraph(
				left, finishY,
				right, startY,
				0, bodyTop,
				imageWidth, bodyBottom - bodyTop,
				imageHandle_, true
			);
		}

		// 上のバー：押し終える位置
		DrawRectExtendGraph(
			left, finishY - NOTES_HALF_HEIGHT,
			right, finishY + NOTES_HALF_HEIGHT,
			0, capTop,
			imageWidth, capBottom - capTop,
			imageHandle_, true
		);

		// 下のバー：押し始める位置
		DrawRectExtendGraph(
			left, startY - NOTES_HALF_HEIGHT,
			right, startY + NOTES_HALF_HEIGHT,
			0, startTop,
			imageWidth, startBottom - startTop,
			imageHandle_, true
		);
	}

	// 他のノーツや画面全体を暗くしないように明るさを戻す
	SetDrawBright(oldRed, oldGreen, oldBlue);

	// 後続の描画のため、元の範囲へ戻す
	SetDrawArea(
		oldArea.left, oldArea.top,
		oldArea.right, oldArea.bottom
	);
}