#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/MusicManager.h"
#include "../CsvData/Song/SongDataLoader.h"
#include "SongSelectScene.h"

SongSelectScene::SongSelectScene(void)
	:
	currentSongIndex_(0)
{
}

SongSelectScene::~SongSelectScene(void)
{
}

void SongSelectScene::Init(void)
{
	// 最初の曲を選択
	currentSongIndex_ = 0;

	// CSVから楽曲データを読み込む
	songs_ = SongDataLoader::Load(
		Application::PATH_CSV + "Song/SongData.csv"
	);

	// 最初の曲を試聴
	if (!songs_.empty())
	{
		PlaySelectedSong();
	}
}

void SongSelectScene::Update(void)
{
	InputManager& inputMng =
		InputManager::GetInstance();

	// タイトルに戻る
	if (IsMouseOnTitleButton() && inputMng.IsTrgMouseLeft())
	{
		scnMng_.ChangeScene(
			SceneManager::SCENE_ID::TITLE
		);

		return;
	}

	// 楽曲データがない場合
	if (songs_.empty())
	{
		return;
	}

	// 前の曲
	if (inputMng.IsTrgDown(KEY_INPUT_LEFT))
	{
		currentSongIndex_--;

		// 最初の曲より前へ行った場合
		if (currentSongIndex_ < 0)
		{
			// 最後の曲へ移動
			currentSongIndex_ =
				static_cast<int>(songs_.size()) - 1;
		}

		// 選択した曲を試聴
		PlaySelectedSong();
	}

	// 次の曲
	if (inputMng.IsTrgDown(KEY_INPUT_RIGHT))
	{
		currentSongIndex_++;

		// 最後の曲より後へ行った場合
		if (currentSongIndex_ >=
			static_cast<int>(songs_.size()))
		{
			// 最初の曲へ戻る
			currentSongIndex_ = 0;
		}

		// 選択した曲を試聴
		PlaySelectedSong();
	}

	// 曲を決定
	if (inputMng.IsTrgDown(KEY_INPUT_RETURN))
	{
		const SongData& song = songs_[currentSongIndex_];

		// 音源未設定の曲は開始しない
		if (song.musicFile.empty())
		{
			return;
		}

		// 選んだ曲を保存
		scnMng_.SetSelectedSong(song);

		// 試聴を停止
		MusicManager::GetInstance().Stop();

		scnMng_.ChangeScene(
			SceneManager::SCENE_ID::GAME
		);
	}
}

void SongSelectScene::Draw(void)
{
	// 背景
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		GetColor(15, 20, 45),
		true
	);

	// 選曲画面タイトル
	DrawString(
		560,
		60,
		"MUSIC SELECT",
		GetColor(255, 255, 255)
	);

	// タイトルへ戻るボタン
	const bool isMouseOnButton =
		IsMouseOnTitleButton();

	DrawBox(
		TITLE_BUTTON_LEFT,
		TITLE_BUTTON_TOP,
		TITLE_BUTTON_RIGHT,
		TITLE_BUTTON_BOTTOM,
		isMouseOnButton
		? GetColor(90, 110, 180)
		: GetColor(40, 50, 90),
		true
	);

	DrawString(
		TITLE_BUTTON_LEFT + 20,
		TITLE_BUTTON_TOP + 15,
		"< タイトルへ戻る",
		GetColor(255, 255, 255)
	);

	// ジャケットの仮枠
	DrawBox(
		JACKET_LEFT,
		JACKET_TOP,
		JACKET_RIGHT,
		JACKET_BOTTOM,
		GetColor(50, 60, 100),
		true
	);

	// CSVが読み込めなかった場合
	if (songs_.empty())
	{
		DrawString(
			520,
			610,
			"楽曲データがありません",
			GetColor(255, 255, 255)
		);

		return;
	}

	const SongData& song =
		songs_[currentSongIndex_];

	// 曲名
	if (!song.title.empty())
	{
		DrawString(
			570,
			610,
			song.title.c_str(),
			GetColor(255, 255, 255)
		);
	}
	else
	{
		DrawString(
			570,
			610,
			"曲名未設定",
			GetColor(180, 180, 180)
		);
	}

	// BPM
	if (song.bpm > 0)
	{
		DrawFormatString(
			570,
			640,
			GetColor(200, 220, 255),
			"BPM : %d",
			song.bpm
		);
	}
	else
	{
		DrawString(
			570,
			640,
			"BPM : 未設定",
			GetColor(180, 180, 180)
		);
	}

	// 前後の曲
	DrawString(
		350,
		350,
		"<",
		GetColor(255, 255, 255)
	);

	DrawString(
		920,
		350,
		">",
		GetColor(255, 255, 255)
	);
}

void SongSelectScene::Release(void)
{
	songs_.clear();
}

void SongSelectScene::PlaySelectedSong(void)
{
	// 楽曲データがない場合
	if (songs_.empty())
	{
		return;
	}

	const SongData& song =
		songs_[currentSongIndex_];

	MusicManager& musicMng =
		MusicManager::GetInstance();

	// MusicFileが未設定の場合
	if (song.musicFile.empty())
	{
		MusicManager::GetInstance().Release();
		return;
	}

	// 読み込みに成功したらループ再生
	if (musicMng.Load(song.musicFile))
	{
		musicMng.Play(true);
	}
}

bool SongSelectScene::IsMouseOnTitleButton(void) const
{
	const Vector2 mousePos = InputManager::GetInstance().GetMousePos();

	return
		mousePos.x >= TITLE_BUTTON_LEFT &&
		mousePos.x <= TITLE_BUTTON_RIGHT &&
		mousePos.y >= TITLE_BUTTON_TOP &&
		mousePos.y <= TITLE_BUTTON_BOTTOM;
}
