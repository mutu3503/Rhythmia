#include <DxLib.h>
#include "../Application.h"
#include "SoundManager.h"

SoundManager& SoundManager::GetInstance(void)
{
    // 初めて呼ばれたときに一回だけ生成
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager(void)
{
    // 読み込み前のハンドルを無効値にする
    tapHandles_.fill(INVALID_SOUND_HANDLE);
    holdHandles_.fill(INVALID_SOUND_HANDLE);
}

bool SoundManager::Init(void)
{
	// すでに初期化済みなら何もしない
	if (isInitialized_)
	{
		return true;
	}

	// 通常ノーツSEを読み込む
	const std::string tapPath = Application::PATH_SOUND + TAP_PATH;

	for (int& handle : tapHandles_)
	{
		handle = LoadSoundMem(tapPath.c_str());

		if (handle == INVALID_SOUND_HANDLE)
		{
			Release();
			return false;
		}

		ChangeVolumeSoundMem(TAP_VOLUME, handle);
	}

	// レーンごとに別のハンドルに読み込む
	const std::string holdPath = Application::PATH_SOUND + HOLD_PATH;

	for (int& handle : holdHandles_)
	{
		handle = LoadSoundMem(holdPath.c_str());

		if (handle == INVALID_SOUND_HANDLE)
		{
			Release();
			return false;
		}

		ChangeVolumeSoundMem(HOLD_VOLUME, handle);
	}

	nextTapVoice_ = 0;
	isInitialized_ = true;
	return true;
}

void SoundManager::PlayTap(void)
{
	if (!isInitialized_)
	{
		return;
	}

	// 空いてる再生枠を優先
	int voice = nextTapVoice_;

	for (int offset = 0; offset < TAP_VOICE_COUNT; ++offset)
	{
		const int candidate = (nextTapVoice_ + offset) % TAP_VOICE_COUNT;

		if (CheckSoundMem(tapHandles_[candidate]) == 0)
		{
			voice = candidate;
			break;
		}
	}

	// 全ての枠が埋まっていたら再利用する
	PlaySoundMem(tapHandles_[voice], DX_PLAYTYPE_BACK, true);

	nextTapVoice_ = (voice + 1) % TAP_VOICE_COUNT;
}

void SoundManager::StartHold(int laneIdx)
{
	if (!isInitialized_ || laneIdx < 0 || laneIdx >= LANE_COUNT)
	{
		return;
	}

	const int handle = holdHandles_[laneIdx];

	// 再生中なら先頭に戻さない
	if (CheckSoundMem(handle) != 1)
	{
		PlaySoundMem(handle, DX_PLAYTYPE_LOOP, true);
	}
}

void SoundManager::StopHold(int laneIdx)
{
	if (!isInitialized_ || laneIdx < 0 || laneIdx >= LANE_COUNT)
	{
		return;
	}

	// このレーンのロングノーツSEだけを停止
	StopSoundMem(holdHandles_[laneIdx]);
}

void SoundManager::StopAllSE(void)
{
	// 読み込み途中でも停止できるようにする
	for (int handle : tapHandles_)
	{
		if (handle != INVALID_SOUND_HANDLE)
		{
			StopSoundMem(handle);
		}	
	}
}

void SoundManager::Release(void)
{
	StopAllSE();

	// 取得済みのハンドルを解放
	for (int& handle : tapHandles_)
	{
		if (handle != INVALID_SOUND_HANDLE)
		{
			DeleteSoundMem(handle);
			handle = INVALID_SOUND_HANDLE;
		}
	}

	for (int& handle : holdHandles_)
	{
		if (handle != INVALID_SOUND_HANDLE)
		{
			DeleteSoundMem(handle);
			handle = INVALID_SOUND_HANDLE;
		}
	}

	nextTapVoice_ = 0;
	isInitialized_ = false;
}