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
	constexpr int GUIDE_Y = 460;

	// リザルト画面の文字色
	constexpr int TEXT_WHITE = 255;

	// スコアと判定欄の表示位置・行間（ピクセル）。
	constexpr int SCORE_Y = 220;
	constexpr int JUDGMENT_TOP_Y = 280;
	constexpr int JUDGMENT_LINE_SPACING = 36;
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

	// 曲名とプレイ終了時に受け取った結果を参照
	const auto& song = scnMng_.GetSelectedSong();
	const auto& result = scnMng_.GetPlayResult();

	DrawString(TEXT_X, HEADING_Y, "RESULT", textColor);
	DrawString(TEXT_X, SONG_TITLE_Y, song.title.c_str(), textColor);
	DrawFormatString(TEXT_X, SCORE_Y, textColor, "SCORE: %d", result.score);

	// 判定名と件数を組にして同じ間隔で並べる
	const struct JudgmentRow
	{
		const char* label;
		int count;
	} rows[] = {
		{ "PERFECT", result.perfectCount },
		{ "GREAT",   result.greatCount },
		{ "GOOD",    result.goodCount },
		{ "MISS",    result.missCount },
	};

	int y = JUDGMENT_TOP_Y;
	for (const auto& row : rows)
	{
		DrawFormatString(TEXT_X, y, textColor, "%s: %d", row.label, row.count);
		y += JUDGMENT_LINE_SPACING;
	}

	DrawString(TEXT_X, GUIDE_Y, "BackSpace: 選曲へ戻る", textColor);
}

void ResultScene::Release(void)
{
}
