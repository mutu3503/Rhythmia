#pragma once
#include <vector>
#include "../../Manager/ResourceManager.h"
#include "../Notes/NotesBase.h"

class Lane
{
public:

	// 配置よ担当するキーの表示名
	void Init(int left, int top,
		int width, int bottom, int judgeY,
		const char* keyName, int laneIdx);

	// 曲の時刻を使って全てのノーツを更新
	void Update(double musicTimeSeconds);

	void Draw(void) const;

	// レーンに通常ノーツを追加
	void AddNotes(double hitTimeSeconds, int imageHandle);

	// レーンにロングノーツを追加
	void AddHoldNotes(double hitTimeSeconds, double endTimeSeconds,
		int imageHandle);

	// 押した瞬間の入力と曲の時刻を使いノーツの判定をする
	void Judge(double musicTimeSeconds, bool isKeyPressed,
		bool isKeyHeld, std::vector<JUDGMENT>& finishedJudgment);

private :

	// レーン背景色
	static constexpr int BACKGROUND_R = 35;
	static constexpr int BACKGROUND_G = 45;
	static constexpr int BACKGROUND_B = 70;

	// レーン枠の色
	static constexpr int BORDER_R = 120;
	static constexpr int BORDER_G = 140;
	static constexpr int BORDER_B = 180;

	// キー名の色
	static constexpr int KEY_TEXT_R = 255;
	static constexpr int KEY_TEXT_G = 255;
	static constexpr int KEY_TEXT_B = 255;

	// キー名の表示位置（レーン左端・下端からの距離）
	static constexpr int KEY_TEXT_OFFSET_X = 52;
	static constexpr int KEY_TEXT_BOTTOM_MARGIN = 30;

	// 判定ラインの色と太さ（ピクセル）
	static constexpr int JUDGE_LINE_R = 255;
	static constexpr int JUDGE_LINE_G = 220;
	static constexpr int JUDGE_LINE_B = 80;
	static constexpr int JUDGE_LINE_THICKNESS = 3;

	// 入力時刻と判定時刻の差の許容幅
	static constexpr double PERFECT_WINDOW_SECONDS = 0.050;
	static constexpr double GREAT_WINDOW_SECONDS = 0.100;
	static constexpr double GOOD_WINDOW_SECONDS = 0.150;

	// 判定画像の表示時間
	static constexpr double JUDGMENT_DISPLAY_SECONDS = 0.5;

	// 判定画像の配置と大きさ
	static constexpr int JUDGMENT_SIDE_MARGIN = 4;
	static constexpr int JUDGMENT_HEIGHT = 55;
	static constexpr int JUDGMENT_BOTTOM_OFFSET = 90;

	// 画像ハンドルの無効値
	static constexpr int INVALID_JUDGMENT_HANDLE = -1;

	// 終点直前にキーを離しても成功にする
	static constexpr double HOLD_RELEASE_GRACE_SECONDS = 0.080;

	// このレーンに属するノーツ
	std::vector<NotesBase> notes_;

	// レーンの配置
	int left_ = 0;
	int top_ = 0;
	int width_ = 0;
	int bottom_ = 0;
	int judgeY_ = 0;
	int laneIdx_ = 0;

	// 現在表示している判定画像
	int judgmentImageHandle_ = INVALID_JUDGMENT_HANDLE;

	// 判定画像の表示を終了する曲の時刻
	double judgmentEndSeconds_ = 0.0;

	// 担当キーの表示名
	const char* keyName_ = "";

	// 指定された判定画像を一定時間表示
	void ShowJudgment(ResourceManager::SRC image,
		double musicTimeSeconds
	);
};

