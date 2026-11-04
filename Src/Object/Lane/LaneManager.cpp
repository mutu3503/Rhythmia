#include <cmath>
#include "../../Manager/InputManager.h"
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
			JUDGE_LINE_Y, keyNames_[laneIdx]
		);

		// レーンで使用するキーを入力管理に登録
		InputManager::GetInstance().Add(laneKeys_[laneIdx]);
	}
}

void LaneManager::Update(double musicTimeSeconds, bool enableJudgment)
{
	InputManager& inputMng =
		InputManager::GetInstance();

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
				isKeyHeld
			);
		}
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

		if (data.type == NOTE_TYPE::HOLD_NOTE)
		{
			// 終点にも、同じBPMと調整値を適用する
			const double endTimeSeconds =
				firstBeatSeconds + data.endBeat * secondsPerBeat;

			lanes_[data.lane].AddHoldNotes(
				hitTimeSeconds,
				endTimeSeconds,
				holdImageHandle
			);
		}
		else
		{
			lanes_[data.lane].AddNotes(
				hitTimeSeconds,
				normalImageHandle
			);
		}
	}

	Update(MUSIC_START_SECONDS, false);

	return true;
}
