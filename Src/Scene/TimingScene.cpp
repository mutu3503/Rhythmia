#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "TimingScene.h"

namespace
{
	// 背景の左上座標と、文字の表示位置・行間（ピクセル）。
	constexpr int SCREEN_ORIGIN = 0;
	constexpr int TEXT_LEFT = 100;
	constexpr int TEXT_TOP = 100;
	constexpr int LINE_SPACING = 42;

	// 背景色と文字色（RGB、各0～255）。
	constexpr int BACKGROUND_R = 15;
	constexpr int BACKGROUND_G = 20;
	constexpr int BACKGROUND_B = 45;
	constexpr int TEXT_WHITE = 255;
}

void TimingScene::Init(void)
{
	// 設定画面を開くたびに前回保存した値を読み込む
	offsetMs_ = TimingSettings::LoadMilliseconds();
	statusText_ = "";

	// この画面で使うキーを入力管理に登録
	auto& input = InputManager::GetInstance();
	const int keys[] =
	{
		KEY_INPUT_LEFT,
		KEY_INPUT_RIGHT,
		KEY_INPUT_R,
		KEY_INPUT_RETURN,
		KEY_INPUT_BACK,
	};

	for (int key : keys)
	{
		input.Add(key);
	}
}

void TimingScene::Update(void)
{
	auto& input = InputManager::GetInstance();

	// 保存していない変更は破棄
	if (input.IsTrgDown(KEY_INPUT_BACK))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::SETTING);
		return;
	}

	// 左で早く、右で遅くする。値は保存できる範囲に収める。
	if (input.IsTrgDown(KEY_INPUT_LEFT))
	{
		offsetMs_ = TimingSettings::ClampMilliseconds(
			offsetMs_ - OFFSET_STEP_MS
		);
		statusText_ = "";
	}
	else if (input.IsTrgDown(KEY_INPUT_RIGHT))
	{
		offsetMs_ = TimingSettings::ClampMilliseconds(
			offsetMs_ + OFFSET_STEP_MS
		);
		statusText_ = "";
	}

	// Rキーで補正値を初期値に戻す
	if (input.IsTrgDown(KEY_INPUT_R))
	{
		offsetMs_ = TimingSettings::DEFAULT_OFFSET_MS;
		statusText_ = "";
	}

	if (input.IsTrgDown(KEY_INPUT_RETURN))
	{
		// 書き込みに成功した場合だけ保存完了を表示
		statusText_ = TimingSettings::SaveMilliseconds(offsetMs_)
			? "保存しました。次のプレイから反映されます。"
			: "設定を保存できませんでした";
	}
}

void TimingScene::Draw(void)
{
	// 背景を先に描き、調整値と操作案内を重ねる
	DrawBox(
		SCREEN_ORIGIN, SCREEN_ORIGIN,
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
		GetColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B),
		true
	);

	const auto textColor = GetColor(TEXT_WHITE, TEXT_WHITE, TEXT_WHITE);
	int y = TEXT_TOP;

	DrawString(TEXT_LEFT, y, "タイミング調整", textColor);
	y += LINE_SPACING;

	// 正負の符号も表示し、早める・遅らせる方向を示す
	DrawFormatString(TEXT_LEFT, y, textColor, "補正値: %+d ms", offsetMs_);
	y += LINE_SPACING;

	DrawString(TEXT_LEFT, y, "左: 早く    右: 遅く    R: リセット", textColor);
	y += LINE_SPACING;

	DrawString(TEXT_LEFT, y, "Enter: 保存    BackSpace: 設定へ戻る", textColor);
	y += LINE_SPACING;

	DrawString(TEXT_LEFT, y, statusText_, textColor);
}

void TimingScene::Release(void)
{
}
