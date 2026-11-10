#pragma once

class NoteSpeedSettings
{
public:

	// 速度の設定値
	static constexpr int DEFAULT_PERCENT = 100;
	static constexpr int MIN_PERCENT = 50;
	static constexpr int MAX_PERCENT = 200;
	static constexpr int STEP_PERCENT = 10;

	static int ClampPercent(int percent);
	static int LoadPercent(void);
	static bool SavePercent(int percent);

	// ノーツの位置計算に使う速度
	static double LoadPixelsPerSeconds(void);

private:

	// 100%の時の落下速度
	static constexpr double BASE_PIXELS_PER_SECOND = 300.0;

};

