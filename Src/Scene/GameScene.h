#pragma once
#include <vector>
#include "../Object/Lane/LaneManager.h"
#include "../CsvData/Chart/ChartData.h"
#include "../CsvData/Song/SongData.h"
#include "SceneBase.h"

class GameScene : public SceneBase
{
public:

	GameScene(void);			// コンストラクタ
	~GameScene(void) override;	// デストラクタ

	void Init(void) override;	// 初期化
	void Update(void) override;	// 更新
	void Draw(void) override;	// 描画
	void Release(void) override;// 解放

private:

	// ゲーム中の全レーンを管理
	LaneManager laneManager_;

	// 楽曲の開始時刻
	static constexpr double MUSIC_START_SECONDS = 0.0;

	// 画面の左上座標（ピクセル）
	static constexpr int SCREEN_LEFT = 0;
	static constexpr int SCREEN_TOP = 0;

	// 背景色
	static constexpr int BACKGROUND_R = 15;
	static constexpr int BACKGROUND_G = 20;
	static constexpr int BACKGROUND_B = 45;

	// 楽曲情報・操作案内の表示位置（ピクセル）
	static constexpr int INFO_TEXT_X = 40;
	static constexpr int TITLE_TEXT_Y = 30;
	static constexpr int BPM_TEXT_Y = 60;
	static constexpr int ERROR_TEXT_Y = 100;
	static constexpr int MUSIC_TIME_TEXT_Y = 130;
	static constexpr int GUIDE_TEXT_Y = 680;

	// 曲名・再生時刻の表示色
	static constexpr int MAIN_TEXT_R = 255;
	static constexpr int MAIN_TEXT_G = 255;
	static constexpr int MAIN_TEXT_B = 255;

	// BPMの表示色
	static constexpr int BPM_TEXT_R = 200;
	static constexpr int BPM_TEXT_G = 220;
	static constexpr int BPM_TEXT_B = 255;

	// エラーの表示色
	static constexpr int ERROR_TEXT_R = 255;
	static constexpr int ERROR_TEXT_G = 100;
	static constexpr int ERROR_TEXT_B = 100;

	// 操作案内の表示色
	static constexpr int GUIDE_TEXT_R = 200;
	static constexpr int GUIDE_TEXT_G = 200;
	static constexpr int GUIDE_TEXT_B = 200;

	// 楽曲を再生するまでの時間
	static constexpr double MUSIC_START_DELAY_SECONDS = 2.0;

	// 再生までに経過した時間
	double startWaitSeconds_ = MUSIC_START_SECONDS;

	// 譜面の読み込み結果の表示位置
	static constexpr int CHART_INFO_TEXT_Y = 160;

	// 読み込んだ譜面データ
	std::vector<ChartData> chartDataList_;

	// 選んだ曲
	SongData song_{};

	// ノーツの移動と入力判定で共通して使用する楽曲の時刻
	double musicTime_ = MUSIC_START_SECONDS;

	// 再生開始処理を行ったか
	bool hasStarted_ = false;

	// 読み込みに成功したか
	bool isMusicLoaded_ = false;

	// 譜面の読み込みに成功したか
	bool isChartLoaded_ = false;
};

