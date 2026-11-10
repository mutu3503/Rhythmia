#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "ResultScene.h"

namespace
{
	// 文字の表示位置
	constexpr int TEXT_X = 100;
	constexpr int HEADING_Y = 100;
	constexpr int SONG_TITLE_Y = 160;
	constexpr int GUIDE_Y = 240;

	// リザルト画面の文字色
	constexpr int TEXT_WHITE = 255;
}

void ResultScene::Init(void)
{
	// 選曲画面に戻るキー
	InputManager::GetInstance().Add(KEY_INPUT_BACK);
}

void ResultScene::Update(void)
{
	if(InputManager::GetInstance().IsTrgDown(KEY_INPUT_BACK))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::SONG_SELECT);
	}
}

void ResultScene::Draw(void)
{
	const auto textColor = GetColor(TEXT_WHITE, TEXT_WHITE, TEXT_WHITE);
	const auto& song = scnMng_.GetSelectedSong();

	DrawString(TEXT_X, HEADING_Y, "RESULT", textColor);
	DrawString(TEXT_X, SONG_TITLE_Y, song.title.c_str(), textColor);
	DrawString(TEXT_X, GUIDE_Y, "BackSpace: 選曲へ戻る", textColor);
}

void ResultScene::Release(void)
{
}
