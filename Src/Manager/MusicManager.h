#pragma once
#include <string>

class MusicManager
{
public:

	// インスタンス生成
	static void CreateInstance(void);

	// インスタンス取得
	static MusicManager& GetInstance(void);

	// 楽曲を読み込む
	bool Load(const std::string& musicFile);

	// 楽曲を再生
	void Play(bool isLoop);

	// 楽曲を停止
	void Stop(void);

	// 解放
	void Release(void);

	// 破棄
	void Destroy(void);

	// 楽曲が読み込まれているか
	bool IsLoaded(void) const;

	// 曲の再生位置を秒で取得
	double GetPlaybackSeconds(void) const;

private:

	// ミリ秒から秒への変換に使用する値
	static constexpr double MILLISECONDS_PER_SECOND = 1000.0;

	// 再生位置を取得できなかった場合の値
	static constexpr double INVALID_PLAYBACK_SECONDS = -1.0;

	// 現在読み込んでいるファイル名
	std::string currentMusicFile_;

	// 読み込み失敗時のサウンドハンドル
	static constexpr int INVALID_SOUND_HANDLE = -1;

	// シングルトン用インスタンス
	static MusicManager* instance_;

	// 現在読み込んでいる楽曲
	int musicHandle_;

	// 外部から生成できないようにする
	MusicManager(void);

	~MusicManager(void) = default;
};