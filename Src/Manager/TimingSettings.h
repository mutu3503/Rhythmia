#pragma once

class TimingSettings
{
public:

	// 未設定の場合は補正しない
	static constexpr int DEFAULT_OFFSET_MS = 0;

	// 保存された補正値を読み込む
	static int LoadMilliseconds();

	// 補正値を保存する
	static bool SaveMilliseconds(int milliseconds);

	// ゲームシーンで使用する補正値を取得する
	static double LoadSeconds();

	// 読み込み時と保存時に同じ補正範囲を適応
	static int ClampMilliseconds(int milliseconds);

private:

	// 手動で設定できる範囲は±300ミリ秒
	static constexpr int MAX_OFFSET_MS = 300;

	// ミリ秒から秒への変換に使用
	static constexpr double MILLISECONDS_PER_SECOND = 1000.0;
};

