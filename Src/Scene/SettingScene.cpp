#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/NoteSpeedSettings.h"
#include "../Manager/ResourceManager.h"
#include "SettingScene.h"

namespace
{
	// 背景とメニューの配置（ピクセル）。
	constexpr int SCREEN_ORIGIN = 0;
	constexpr int TEXT_LEFT = 100;
	constexpr int TITLE_Y = 100;
	constexpr int TIMING_ITEM_Y = 180;
	constexpr int SPEED_ITEM_Y = 230;
	constexpr int BACK_ITEM_Y = 280;
	constexpr int GUIDE_Y = 360;
	constexpr int STATUS_Y = 410;

	// 背景色と文字色（RGB、各0～255）。
	constexpr int BACKGROUND_R = 15;
	constexpr int BACKGROUND_G = 20;
	constexpr int BACKGROUND_B = 45;
	constexpr int WHITE = 255;
	constexpr int SELECTED_R = 100;
	constexpr int SELECTED_G = 220;
	constexpr int SELECTED_B = 255;
}

void SettingScene::Init(void)
{
	selectedItem_ = MenuItem::Timing;

	// 設定を開いたときは前回保存した速度を表示
	noteSpeedPercent_ = NoteSpeedSettings::LoadPercent();

	// プレイ画面と同じ通常ノーツを使う
	const int noteImageHandle = resMng_.Load(
		ResourceManager::SRC::NORMAL_NOTES
	).handleId_;

	noteSpeedPreview_.Init(noteImageHandle);

	speedStatusText_ = "";

	// この画面で使うキーを入力管理を登録
	auto& input = InputManager::GetInstance();
	const int keys[] =
	{
		KEY_INPUT_UP,
		KEY_INPUT_DOWN,
		KEY_INPUT_LEFT,
		KEY_INPUT_RIGHT,
		KEY_INPUT_RETURN,
		KEY_INPUT_BACK,
	};

	for (int key : keys)
	{
		input.Add(key);
	}
}

void SettingScene::Update(void)
{
	auto& input = InputManager::GetInstance();

	// バックスペースキーを押したらタイトルに戻る
	if (input.IsTrgDown(KEY_INPUT_BACK))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		return;
	}

	// 上下キーで項目を選ぶ
	const int itemCount = static_cast<int>(MenuItem::Count);
	int itemIdx = static_cast<int>(selectedItem_);

	if (input.IsTrgDown(KEY_INPUT_UP))
	{
		itemIdx = (itemIdx + itemCount - 1) % itemCount;
	}
	else if(input.IsTrgDown(KEY_INPUT_DOWN))
	{
		itemIdx = (itemIdx + 1) % itemCount;
	}

	selectedItem_ = static_cast<MenuItem>(itemIdx);

	if (selectedItem_ == MenuItem::NoteSpeed)
	{
		// 速度の項目を選んでいるときだけ左右キーで変更
		const int step = NoteSpeedSettings::STEP_PERCENT;

		if (input.IsTrgDown(KEY_INPUT_LEFT))
		{
			noteSpeedPercent_ = NoteSpeedSettings::ClampPercent(
				noteSpeedPercent_ - step
			);
			speedStatusText_ = "";
		}
		else if (input.IsTrgDown(KEY_INPUT_RIGHT))
		{
			noteSpeedPercent_ = NoteSpeedSettings::ClampPercent(
				noteSpeedPercent_ + step
			);
			speedStatusText_ = "";
		}
	}

	// Enterキーで選択した項目を実行
	if (!input.IsTrgDown(KEY_INPUT_RETURN))
	{
		return;
	}

	switch (selectedItem_)
	{
	case SettingScene::MenuItem::Timing:
		scnMng_.ChangeScene(SceneManager::SCENE_ID::TIMING);
		break;
	case SettingScene::MenuItem::NoteSpeed:
		speedStatusText_ = NoteSpeedSettings::SavePercent(noteSpeedPercent_)
			? "ノーツ速度を保存しました"
			: "ノーツ速度を保存できませんでした";
		break;
	case SettingScene::MenuItem::Back:
		scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		break;
	case SettingScene::MenuItem::Count:
		break;
	default:
		break;
	}
}

void SettingScene::Draw(void)
{
	// 背景を描画しその上にメニューを描画する
	DrawBox(SCREEN_ORIGIN, SCREEN_ORIGIN,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		GetColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B),
		true
	);

	const auto normalColor = GetColor(WHITE, WHITE, WHITE);
	const auto selectedColor = GetColor(SELECTED_R, SELECTED_G, SELECTED_B);

	DrawString(TEXT_LEFT, TITLE_Y, "設定", normalColor);

	// 選択中の項目は＞と色を変えて表示する
	const bool timingSelected = selectedItem_ == MenuItem::Timing;
	const bool speedSelected = selectedItem_ == MenuItem::NoteSpeed;
	const bool backSelected = selectedItem_ == MenuItem::Back;

	DrawString(
		TEXT_LEFT, TIMING_ITEM_Y,
		timingSelected ? "> タイミング調整" : "  タイミング調整",
		timingSelected ? selectedColor : normalColor
	);

	DrawFormatString(
		TEXT_LEFT, SPEED_ITEM_Y,
		speedSelected ? selectedColor : normalColor,
		"%sノーツ速度 : %d%%",
		speedSelected ? ">" : " ",
		noteSpeedPercent_
	);

	DrawString(
		TEXT_LEFT, BACK_ITEM_Y,
		backSelected ? "> タイトルへ戻る" : "  タイトルへ戻る",
		backSelected ? selectedColor : normalColor
	);

	DrawString(
		TEXT_LEFT, GUIDE_Y,
		"上下: 選択    左右: 速度変更    Enter: 決定・保存",
		normalColor
	);

	DrawString(TEXT_LEFT, STATUS_Y, speedStatusText_, normalColor);

	// 画面で編集中の値を全て反映
	noteSpeedPreview_.Draw(
		NoteSpeedSettings::ToPixelsPerSecond(noteSpeedPercent_)
	);
}

void SettingScene::Release(void)
{
}
