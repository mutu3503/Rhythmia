#include <DxLib.h>
#include "../Application.h"
#include "MusicManager.h"

// シングルトン用
MusicManager* MusicManager::instance_ = nullptr;

void MusicManager::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new MusicManager();
    }
}

MusicManager& MusicManager::GetInstance(void)
{
    if (instance_ == nullptr)
    {
        CreateInstance();
    }

    return *instance_;
}

MusicManager::MusicManager(void)
    :
    currentMusicFile_(),
    musicHandle_(INVALID_SOUND_HANDLE)
{
}

bool MusicManager::Load(const std::string& musicFile)
{
    // ファイル名が空なら読み込まない
    if (musicFile.empty())
    {
        return false;
    }

    // 同じ曲が読み込まれている場合
    if (currentMusicFile_ == musicFile &&
        musicHandle_ != INVALID_SOUND_HANDLE)
    {
        return true;
    }

    // 前に読み込んでいた楽曲を解放
    Release();

    // 楽曲パスを作る
    const std::string filePath = Application::PATH_SOUND +
        "Music/" + musicFile;

    // 楽曲を読み込む
    musicHandle_ = LoadSoundMem(filePath.c_str());

    // 読み込み失敗時
    if (musicHandle_ == INVALID_SOUND_HANDLE)
    {
        currentMusicFile_.clear();
        return false;
    }

    // ファイル名を保存
    currentMusicFile_ = musicFile;
    
    return true;
}

void MusicManager::Play(bool isLoop)
{
    // 楽曲情報がない場合は何もしない
    if (!IsLoaded())
    {
        return;
    }

    // 選曲画面ではループ
    const int playType = isLoop
        ? DX_PLAYTYPE_LOOP
        : DX_PLAYTYPE_BACK;

    PlaySoundMem(musicHandle_, playType, true);
}

void MusicManager::Stop(void)
{
    if (!IsLoaded())
    {
        return;
    }

    StopSoundMem(musicHandle_);
}

void MusicManager::Release(void)
{
    if (!IsLoaded())
    {
        return;
    }

    // 再生中なら停止
    Stop();

    // サウンドデータを解放
    DeleteSoundMem(musicHandle_);

    musicHandle_ = INVALID_SOUND_HANDLE;

    currentMusicFile_.clear();
}

void MusicManager::Destroy(void)
{
    Release();

    delete instance_;
    instance_ = nullptr;
}

bool MusicManager::IsLoaded(void) const
{
    return musicHandle_ != INVALID_SOUND_HANDLE;
}

double MusicManager::GetPlaybackSeconds(void) const
{
    if (!IsLoaded())
    {
        return INVALID_PLAYBACK_SECONDS;
    }

    const auto timeMs = GetSoundCurrentTime(musicHandle_);

    if (timeMs < 0)
    {
        return INVALID_PLAYBACK_SECONDS;
    }

    return static_cast<double>(timeMs) / MILLISECONDS_PER_SECOND;
}
