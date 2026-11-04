#include <DxLib.h>
#include <cmath>
#include "Lane.h"

void Lane::Init(int left, int top, 
	int width, int bottom, int judgeY,
	const char* keyName)
{
	// LaneManagerから受け取った配置を保存する
	left_ = left;
	top_ = top;
	width_ = width;
	bottom_ = bottom;
	judgeY_ = judgeY;
	keyName_ = keyName;
	
	// 前回の譜面を残さない
	notes_.clear();

	// 前回の判定表示を残さない
	judgmentImageHandle_ = INVALID_JUDGMENT_HANDLE;
	judgmentEndSeconds_ = 0.0;
}

void Lane::Update(double musicTimeSeconds)
{
	// 全てのノーツを同じ曲の時刻で更新
	for (NotesBase& note : notes_)
	{
		note.Update(musicTimeSeconds, judgeY_);
	}
}

void Lane::Draw(void) const
{
	// 右端を求める
	const int right = left_ + width_;

		// レーンの背景
	DrawBox(
		left_, top_,
		right, bottom_,
		GetColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B),
		true
	);

	// レーンの枠
	DrawBox(
		left_, top_,
		right, bottom_,
		GetColor(BORDER_R, BORDER_G, BORDER_B),
		false
	);

	// 担当キーの表示
	DrawString(
		left_ + KEY_TEXT_OFFSET_X,
		bottom_ - KEY_TEXT_BOTTOM_MARGIN,
		keyName_,
		GetColor(KEY_TEXT_R, KEY_TEXT_G, KEY_TEXT_B)
	);

	// ノーツの描画
	for (const NotesBase& note : notes_)
	{
		note.Draw(left_, width_, top_, bottom_);
	}

	// レーンの判定ライン
	DrawLine(
		left_, judgeY_,
		right, judgeY_,
		GetColor(JUDGE_LINE_R, JUDGE_LINE_G, JUDGE_LINE_B),
		JUDGE_LINE_THICKNESS
	);

	// 判定画像はノーツや判定ラインより後に描画
	if (judgmentImageHandle_ != INVALID_JUDGMENT_HANDLE)
	{
		const int judgmentBottom =
			judgeY_ - JUDGMENT_BOTTOM_OFFSET;

		DrawExtendGraph(
			left_ + JUDGMENT_SIDE_MARGIN,
			judgmentBottom - JUDGMENT_HEIGHT,
			right - JUDGMENT_SIDE_MARGIN,
			judgmentBottom,
			judgmentImageHandle_,
			true
		);
	}
}

void Lane::AddNotes(double hitTimeSeconds, int imageHandle)
{
	// 判定時刻と共有画像を指定してノーツを作る
	notes_.emplace_back(hitTimeSeconds, imageHandle);
}

void Lane::AddHoldNotes(double hitTimeSeconds, double endTimeSeconds, int imageHandle)
{
	// 開始と終了の時刻を持つロングノーツを作る
	notes_.emplace_back(
		hitTimeSeconds,
		endTimeSeconds,
		imageHandle
	);
}

void Lane::Judge(double musicTimeSeconds, bool isKeyPressed, bool isKeyHeld)
{
	// 表示時間を過ぎた判定画像を消す
	if (musicTimeSeconds >= judgmentEndSeconds_)
	{
		judgmentImageHandle_ = INVALID_JUDGMENT_HANDLE;
	}

	auto noteIt = notes_.begin();

	while (noteIt != notes_.end())
	{
		if (noteIt->IsHolding())
		{
			const double endSeconds =
				noteIt->GetEndTimeSeconds();

			// 終点まで保持、または猶予内で離したら完了
			const bool completed =
				musicTimeSeconds >= endSeconds
				|| (!isKeyHeld
					&& musicTimeSeconds >=
					endSeconds - HOLD_RELEASE_GRACE_SECONDS);

			if (completed)
			{
				// 長押し完了をPERFECTとして表示する
				ShowJudgment(
					ResourceManager::SRC::JUDGE_PERFECT,
					musicTimeSeconds
				);

				noteIt = notes_.erase(noteIt);
				continue;
			}

			// 終点より早くキーを離したらMISS
			if (!isKeyHeld)
			{
				ShowJudgment(
					ResourceManager::SRC::JUDGE_MISS,
					musicTimeSeconds
				);

				noteIt = notes_.erase(noteIt);
				continue;
			}
		}
		else
		{
			// 始点を押さないまま通過したノーツはMISS
			const double lateSeconds =
				musicTimeSeconds - noteIt->GetHitTimeSeconds();

			if (lateSeconds > GOOD_WINDOW_SECONDS)
			{
				ShowJudgment(
					ResourceManager::SRC::JUDGE_MISS,
					musicTimeSeconds
				);

				noteIt = notes_.erase(noteIt);
				continue;
			}
		}

		++noteIt;
	}

	if (!isKeyPressed)
	{
		return;
	}

	// まだ押されていない、最も近い始点を探す
	auto targetIt = notes_.end();
	double closestDifference = GOOD_WINDOW_SECONDS;

	for (auto it = notes_.begin(); it != notes_.end(); ++it)
	{
		// 長押し中のノーツをもう一度押す対象にはしない
		if (it->IsHolding())
		{
			continue;
		}

		const double difference = std::abs(
			it->GetHitTimeSeconds() - musicTimeSeconds
		);

		if (difference <= GOOD_WINDOW_SECONDS
			&& (targetIt == notes_.end()
				|| difference < closestDifference))
		{
			targetIt = it;
			closestDifference = difference;
		}
	}

	// ノーツのない場所で押してもペナルティなし
	if (targetIt == notes_.end())
	{
		return;
	}

	ResourceManager::SRC judgment =
		ResourceManager::SRC::JUDGE_GOOD;

	if (closestDifference <= PERFECT_WINDOW_SECONDS)
	{
		judgment = ResourceManager::SRC::JUDGE_PERFECT;
	}
	else if (closestDifference <= GREAT_WINDOW_SECONDS)
	{
		judgment = ResourceManager::SRC::JUDGE_GREAT;
	}

	// 通常ノーツもロングの始点も、同じ幅で判定する
	ShowJudgment(judgment, musicTimeSeconds);

	if (targetIt->IsHold())
	{
		// ロングは消さずに、長押し状態へ切り替える
		targetIt->BeginHold();

		// このフレームから始点を判定ラインに固定する
		targetIt->Update(musicTimeSeconds, judgeY_);
	}
	else
	{
		// 通常ノーツは成功したら消す
		notes_.erase(targetIt);
	}
}

void Lane::ShowJudgment(ResourceManager::SRC image, double musicTimeSeconds)
{
	// 他クラスから判定画像を取得
	judgmentImageHandle_ =
		ResourceManager::GetInstance().Load(image).handleId_;

	// 新しい判定が出るたびに表示時間を更新
	judgmentEndSeconds_ =
		musicTimeSeconds + JUDGMENT_DISPLAY_SECONDS;
}