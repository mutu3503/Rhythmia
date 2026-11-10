#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/NoteSpeedSettings.h"
#include "SettingScene.h"

namespace
{
	// 背景とメニューの配置（ピクセル）。
	constexpr int SCREEN_ORIGIN = 0;
	constexpr int TEXT_LEFT = 100;
	constexpr int TITLE_Y = 100;
	constexpr int TIMING_ITEM_Y = 180;
	constexpr int BACK_ITEM_Y = 230;
	constexpr int GUIDE_Y = 310;

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
	speedStatusText_ = "";

	// この画面で使うキーを入力管理を登録
	auto& input = InputManager::GetInstance();
	const int keys[] =
	{
		KEY_INPUT_UP,
		KEY_INPUT_DOWN,
		KEY_INPUT_RETURN,
		KEY_INPUT_BACK,
		KEY_INPUT_LEFT,
		KEY_INPUT_RIGHT,
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

	// 上下キーでメニュー項目を選択
	if (input.IsTrgDown(KEY_INPUT_UP) || input.IsTrgDown(KEY_INPUT_DOWN))
	{
		selectedItem_ = selectedItem_ == MenuItem::Timing
			? MenuItem::Back
			: MenuItem::Timing;
	}

	// Enterキーで選択した項目を実行
	if (input.IsTrgDown(KEY_INPUT_RETURN))
	{
		const auto nextScene = selectedItem_ == MenuItem::Timing
			? SceneManager::SCENE_ID::TIMING
			: SceneManager::SCENE_ID::TITLE;

		scnMng_.ChangeScene(nextScene);
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

	DrawString(
		TEXT_LEFT, TIMING_ITEM_Y,
		timingSelected ? "> タイミング調整" : "  タイミング調整",
		timingSelected ? selectedColor : normalColor
	);

	DrawString(
		TEXT_LEFT, BACK_ITEM_Y,
		timingSelected ? "  タイトルへ戻る" : "> タイトルへ戻る",
		timingSelected ? normalColor : selectedColor
	);

	DrawString(
		TEXT_LEFT, GUIDE_Y,
		"上下: 選択    Enter: 決定    BackSpace: 戻る",
		normalColor
	);
}

void SettingScene::Release(void)
{
}
