#include <DxLib.h>
#include "../Application.h"
#include "../CsvData/Chart/ChartDataLoader.h"
#include "../Manager/InputManager.h"
#include "../Manager/MusicManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/TimingSettings.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// 選曲画面で選んだ曲を取得
	song_ = scnMng_.GetSelectedSong();

	// 前回のプレイ状態を初期化
	hasStarted_ = false;
	musicTime_ = MUSIC_START_SECONDS - MUSIC_START_DELAY_SECONDS;
	startWaitSeconds_ = MUSIC_START_SECONDS;

	// レーンの配置と担当キーを初期化
	laneManager_.Init();

	// 試聴を停止して、ゲーム用に読み込む
	MusicManager& musicMng = MusicManager::GetInstance();
	musicMng.Stop();

	// 選んだ楽曲をゲーム用に読み込む
	isMusicLoaded_ = musicMng.Load(song_.musicFile);

	// 選曲画面に戻るキーを登録
	InputManager::GetInstance().Add(KEY_INPUT_BACK);

	// 前回の譜面データと読み込み結果をリセット
	chartDataList_.clear();
	isChartLoaded_ = false;

	// 選択した曲に譜面が設定されている時だけ読み込む
	if (!song_.chartFile.empty())
	{
		// 曲データで指定された譜面パスを組む
		const std::string chartPath =
			Application::PATH_CSV + song_.chartFile;

		isChartLoaded_ = ChartDataLoader::Load(
			chartPath, chartDataList_
		);
	}

	// 曲ごとの調整値とPCの環境補正を合計
	const double chartoffsetSeconds =
		song_.offsetSeconds + TimingSettings::LoadSeconds();

	// 読み込んだ譜面を各レーンに配置
	if (isChartLoaded_)
	{
		ResourceManager& resourceMng =
			ResourceManager::GetInstance();

		// 通常ノーツ画像
		const int normalImageHandle =
			resourceMng.Load(
				ResourceManager::SRC::NORMAL_NOTES
			).handleId_;

		// ロングノーツ画像
		const int holdImageHandle =
			resourceMng.Load(
				ResourceManager::SRC::HOLD_NOTES
			).handleId_;

		isChartLoaded_ = laneManager_.SetChart(
			chartDataList_,
			song_.bpm,
			chartoffsetSeconds,
			normalImageHandle,
			holdImageHandle
		);
	}

	// 最初の時点でノーツを見えないようにする
	if (isChartLoaded_)
	{
		laneManager_.Update(musicTime_, false);
	}

	// 曲を始めるたびに終了状態をリセットし、画像を先に読み込む
	hasChartEnded_ = false;

	finishLogoHandle_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::CLEAR_LOGO
	).handleId_;
}

void GameScene::Update(void)
{
	// BackSpaceで選曲画面に戻る
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_BACK))
	{
		scnMng_.ChangeScene(
			SceneManager::SCENE_ID::SONG_SELECT
		);
		return;
	}

	// 譜面終了後は、曲の経過時間を確認
	if (hasChartEnded_)
	{
		const double elapsedSeconds = std::chrono::duration<double>(
			std::chrono::steady_clock::now() - chartEndedAt_
		).count();

		// 3秒後にFINISHが出るのでさらに1秒見せる
		if (elapsedSeconds >= FINISH_DELAY_SECONDS + FINISH_HOLD_SECONDS)
		{
			scnMng_.ChangeScene(
				SceneManager::SCENE_ID::RESULT,
				RESULT_FADE_OUT_SECONDS
			);
			return;
		}
	}

	// 楽曲が読み込めない場合は再生しない
	if (!isMusicLoaded_)
	{
		return;
	}

	MusicManager& musicMng =
		MusicManager::GetInstance();

	// 曲の先頭から再生
	if (!hasStarted_)
	{
		startWaitSeconds_ +=
			static_cast<double>(scnMng_.GetDeltaTime());

		// 指定した時間に達するまでは再生しない
		if (startWaitSeconds_ < MUSIC_START_DELAY_SECONDS)
		{
			musicTime_ =
				startWaitSeconds_ - MUSIC_START_DELAY_SECONDS;

			if (isChartLoaded_)
			{
				laneManager_.Update(musicTime_, false);
			}

			return;
		}

		// ゲーム中はループ再生しない
		musicMng.Play(false);
		hasStarted_ = true;

		// 待機時間の超過分は曲の時刻に加えず、0秒へ合わせる
		musicTime_ = MUSIC_START_SECONDS;

		if (isChartLoaded_)
		{
			laneManager_.Update(musicTime_, false);
		}
	}

	// 曲の現在位置を秒単位で取得
	const double playbackSeconds =
		musicMng.GetPlaybackSeconds();

	// 再生位置を取得できないフレームは更新しない
	if (playbackSeconds < MUSIC_START_SECONDS)
	{
		return;
	}

	musicTime_ = playbackSeconds;

	if (isChartLoaded_)
	{
		// true：位置更新に加えて、入力・MISS判定も行う
		laneManager_.Update(musicTime_, true);

		// 最後のノーツを処理してから、FINISH表示までの計測を始める
		if (!hasChartEnded_
			&& musicTime_ >= laneManager_.GetChartEndSeconds())
		{
			hasChartEnded_ = true;
			chartEndedAt_ = std::chrono::steady_clock::now();
		}
	}
}

void GameScene::Draw(void)
{
	// 背景は文字やレーンより先に描画
	DrawBox(
		SCREEN_LEFT,
		SCREEN_TOP,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		GetColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B),
		true
	);

	// 楽曲名を表示する
	DrawString(
		INFO_TEXT_X,
		TITLE_TEXT_Y,
		song_.title.c_str(),
		GetColor(MAIN_TEXT_R, MAIN_TEXT_G, MAIN_TEXT_B)
	);

	// 楽曲のBPMを表示する
	DrawFormatString(
		INFO_TEXT_X,
		BPM_TEXT_Y,
		GetColor(BPM_TEXT_R, BPM_TEXT_G, BPM_TEXT_B),
		"BPM : %d",
		song_.bpm
	);

	// レーンの背景・枠・キー名・判定ラインを描画する
	laneManager_.Draw();

	// 楽曲を読み込めなかった場合の案内
	if (!isMusicLoaded_)
	{
		DrawString(
			INFO_TEXT_X,
			ERROR_TEXT_Y,
			"音楽の読み込みに失敗しました",
			GetColor(ERROR_TEXT_R, ERROR_TEXT_G, ERROR_TEXT_B)
		);
	}

	// 同期確認用に、曲の再生時刻を小数点以下3桁で表示する
	DrawFormatString(
		INFO_TEXT_X,
		MUSIC_TIME_TEXT_Y,
		GetColor(MAIN_TEXT_R, MAIN_TEXT_G, MAIN_TEXT_B),
		"Music Time : %.3f sec",
		musicTime_
	);

	// 選曲画面へ戻る操作を表示する
	DrawString(
		INFO_TEXT_X,
		GUIDE_TEXT_Y,
		"BackSpace : 選曲画面へ戻る",
		GetColor(GUIDE_TEXT_R, GUIDE_TEXT_G, GUIDE_TEXT_B)
	);

	// 譜面の読み込み結果を確認
	if (isChartLoaded_)
	{
		// 今回のテスト譜面は小規模なので、表示用にintへ変換する
		DrawFormatString(
			INFO_TEXT_X,
			CHART_INFO_TEXT_Y,
			GetColor(MAIN_TEXT_R, MAIN_TEXT_G, MAIN_TEXT_B),
			"Chart Notes : %d",
			static_cast<int>(chartDataList_.size())
		);
	}
	else
	{
		// ファイルの場所やCSVの内容を確認するためのエラー表示
		DrawString(
			INFO_TEXT_X,
			CHART_INFO_TEXT_Y,
			"譜面の読み込みに失敗しました",
			GetColor(ERROR_TEXT_R, ERROR_TEXT_G, ERROR_TEXT_B)
		);
	}

	if (hasChartEnded_ && finishLogoHandle_ != INVALID_FINISH_LOGO)
	{
		// 曲が再生を終えても、この待ち時間は進む
		const double elapsedSeconds = std::chrono::duration<double>(
			std::chrono::steady_clock::now() - chartEndedAt_
		).count();

		if (elapsedSeconds >= FINISH_DELAY_SECONDS)
		{
			int imageWidth = 0;
			int imageHeight = 0;

			// 元画像の縦横比を保ち、画面中央へ配置する
			if (GetGraphSize(finishLogoHandle_, &imageWidth, &imageHeight) == 0
				&& imageWidth > 0 && imageHeight > 0)
			{
				const int height = FINISH_LOGO_WIDTH * imageHeight / imageWidth;
				const int left = (Application::SCREEN_SIZE_X - FINISH_LOGO_WIDTH) / 2;
				const int top = (Application::SCREEN_SIZE_Y - height) / 2;

				DrawExtendGraph(
					left, top,
					left + FINISH_LOGO_WIDTH, top + height,
					finishLogoHandle_,
					true
				);
			}
		}
	}
}

void GameScene::Release(void)
{
	// プレイ中のSEを停止
	SoundManager::GetInstance().StopAllSE();

	// ゲーム終了時に楽曲を停止
	MusicManager::GetInstance().Stop();
}
