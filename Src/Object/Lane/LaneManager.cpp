#include <cmath>
#include "../../Manager/InputManager.h"
#include "../../Manager/NoteSpeedSettings.h"
#include "LaneManager.h"

void LaneManager::Init(void)
{
	for (int laneIdx = 0; laneIdx < LANE_COUNT; laneIdx++)
	{
		// レーン番号に応じて横にずらす
		const int left =
			LANE_LEFT + laneIdx * LANE_WIDTH;

		// 1本分の配置と担当キーを設定
		lanes_[laneIdx].Init(left,
			LANE_TOP, LANE_WIDTH, LANE_BOTTOM,
			JUDGE_LINE_Y, keyNames_[laneIdx], laneIdx
		);

		// レーンで使用するキーを入力管理に登録
		InputManager::GetInstance().Add(laneKeys_[laneIdx]);
	}
}

void LaneManager::Update(double musicTimeSeconds, bool enableJudgment)
{
	InputManager& inputMng =
		InputManager::GetInstance();

	// 前のフレームで集計した判定を残さない
	finishedJudgments_.clear();

	for (int laneIdx = 0; laneIdx < LANE_COUNT; ++laneIdx)
	{
		// 描画位置を曲の現在時刻から計算
		lanes_[laneIdx].Update(musicTimeSeconds);

		// 再生中だけ入力判定と時間切れ判定を行う
		if (enableJudgment)
		{
			// 押した瞬間を取得する
			const bool isKeyPressed =
				inputMng.IsTrgDown(laneKeys_[laneIdx]);

			// 押し続けている状態を取得する
			const bool isKeyHeld =
				inputMng.IsNew(laneKeys_[laneIdx]);

			lanes_[laneIdx].Judge(
				musicTimeSeconds,
				isKeyPressed,
				isKeyHeld,
				finishedJudgments_
			);
		}
	}

	// 全レーンの判定を1つずつ集計
	for (JUDGMENT judgment : finishedJudgments_)
	{
		scoreManager_.AddJudgment(judgment);
	}
}

void LaneManager::Draw(void) const
{
	// 各レーンに描画を任せる
	for (const Lane& lane : lanes_)
	{
		lane.Draw();
	}
}

bool LaneManager::SetChart(const std::vector<ChartData>& chartDataList,
	int bpm, double firstBeatSeconds, int normalImageHandle, int holdImageHandle)
{
	// 前回のノーツを消す
	Init();

	// 前のプレイの集計を消す
	scoreManager_.Reset();

	// 前の曲の終了時刻を残さない
	chartEndSeconds_ = MUSIC_START_SECONDS;

	// BPMが正の値でなければ秒に変換しない
	if (bpm <= 0
		|| chartDataList.empty()
		|| !std::isfinite(firstBeatSeconds)
		|| normalImageHandle == INVALID_IMAGE_HANDLE)
	{
		return false;
	}

	// ノーツを配置するまえに全行の値を確認
	for (const ChartData& data : chartDataList)
	{
		if (!std::isfinite(data.beat)
			|| data.beat < FIRST_BEAT
			|| data.lane < FIRST_LANE_IDX
			|| data.lane >= LANE_COUNT)
		{
			return false;
		}

		if (data.type == NOTE_TYPE::HOLD_NOTE)
		{
			if (!std::isfinite(data.endBeat)
				|| data.endBeat <= data.beat)
			{
				return false;
			}
		}
		else if (data.type != NOTE_TYPE::NORMAL_NOTE)
		{
			return false;
		}
	}

	// 同じレーンのロング区間内に、別の始点を配置しない
	for (std::size_t i = 0; i < chartDataList.size(); ++i)
	{
		const ChartData& hold = chartDataList[i];

		if (hold.type != NOTE_TYPE::HOLD_NOTE)
		{
			continue;
		}

		for (std::size_t j = 0; j < chartDataList.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			const ChartData& other = chartDataList[j];

			if (hold.lane == other.lane
				&& other.beat >= hold.beat
				&& other.beat <= hold.endBeat)
			{
				return false;
			}
		}
	}

	// 1拍にかかる時間
	const double secondsPerBeat =
		SECONDS_PER_MINUTE / static_cast<double>(bpm);

	for (const ChartData& data : chartDataList)
	{
		// 拍番号を曲の先頭から判定時刻に変換
		const double hitTimeSeconds =
			firstBeatSeconds + data.beat * secondsPerBeat;

		// ロングは終了拍、通常ノーツは判定時刻を終了時刻とする。
		const bool isHold = data.type == NOTE_TYPE::HOLD_NOTE;
		const double noteEndSeconds = isHold
			? firstBeatSeconds + data.endBeat * secondsPerBeat
			: hitTimeSeconds;

		if (isHold)
		{
			lanes_[data.lane].AddHoldNotes(
				hitTimeSeconds,
				noteEndSeconds,
				holdImageHandle
			);
		}
		else
		{
			lanes_[data.lane].AddNotes(hitTimeSeconds, normalImageHandle);
		}

		// 最も遅い終了時刻を、FINISH表示の基準として保存する。
		if (noteEndSeconds > chartEndSeconds_)
		{
			chartEndSeconds_ = noteEndSeconds;
		}
	}

	const double scrollSpeed = NoteSpeedSettings::LoadPixelsPerSeconds();

	for (Lane& lane : lanes_)
	{
		lane.SetScrollSpeed(scrollSpeed);
	}

	Update(MUSIC_START_SECONDS, false);

	return true;
}
