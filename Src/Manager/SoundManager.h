#pragma once
#include <array>

class SoundManager
{
public:

	// 共通のSEを管理するインスタンス取得
	static SoundManager& GetInstance(void);

	// 共通のSEを読み込む
	bool Init(void);

	// 通常ノーツの成功音を一度再生
	void PlayTap(void);

	// 指定レーンのロングノーツSEを開始・停止する
	void StartHold(int laneIdx);
	void StopHold(int laneIdx);

	// シーンを切り替えるときに全てのSEを止める
	void StopAllSE(void);

	// ゲーム終了時に全てのSEを解放する
	void Release(void);

private:

	// サウンドハンドルの無効値
	static constexpr int INVALID_SOUND_HANDLE = -1;

	// ロングノーツSEを個別に再生できるレーン数
	static constexpr int LANE_COUNT = 4;

	// 通常ノーツSEの同時再生枠数
	static constexpr int TAP_VOICE_COUNT = 8;

	// 音量
	static constexpr int TAP_VOLUME = 180;
	static constexpr int HOLD_VOLUME = 90;

	// 相対パス
	static constexpr const char* TAP_PATH =
		"SE/Normal_Notes_SE.mp3";
	static constexpr const char* HOLD_PATH =
		"SE/Rong_Notes_SE.mp3";

	// 通常ノーツSEは連打の余韻を残す
	std::array<int, TAP_VOICE_COUNT> tapHandles_;

	// ロングノーツSEはレーンごとに個別に再生する
	std::array<int, LANE_COUNT> holdHandles_;

	// 次に探す通常ノーツSEの再生枠
	int nextTapVoice_ = 0;

	// 全てのSEが読み込み済みになっているか
	bool isInitialized_ = false;

	// 外部からの生成・コピー禁止
	SoundManager(void);
	~SoundManager(void) = default;
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
};

