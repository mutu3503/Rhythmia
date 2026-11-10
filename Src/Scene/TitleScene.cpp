#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Application.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
	:
	currentExitConfirm_(ExitConfirmType::No),
	currentMenu_(MenuType::GameStart),
	titleBackgroundHandle_(INVALID_GRAPH_HANDLE),
	titleLogoHandle_(INVALID_GRAPH_HANDLE),
	isExitConfirm_(false)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	// 最初はゲームスタートを選択
	currentMenu_ = MenuType::GameStart;

	// タイトル背景画像の読み込み
	titleBackgroundHandle_ =
		resMng_.Load(ResourceManager::SRC::TITLE_BACKGROUND).handleId_;

	// タイトルロゴ画像の読み込み
	titleLogoHandle_ =
		resMng_.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	// タイトルパーティクルを生成
	titleParticles_.resize(TITLE_PARTICLE_COUNT);

	for (auto& particle : titleParticles_)
	{
		particle.Init(
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y
		);
	}
}

void TitleScene::Update(void)
{
	// タイトルパーティクルを更新
	for (auto& particle : titleParticles_)
	{
		particle.Update(
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y
		);
	}

	InputManager& inputMng =
		InputManager::GetInstance();

	// 終了確認画面を表示している場合
	if (isExitConfirm_)
	{
		// 左キーでYESを選択
		if (inputMng.IsTrgDown(KEY_INPUT_LEFT))
		{
			currentExitConfirm_ = ExitConfirmType::Yes;
		}
		// 右キーでNOを選択
		else if (inputMng.IsTrgDown(KEY_INPUT_RIGHT))
		{
			currentExitConfirm_ = ExitConfirmType::No;
		}

		// Enterキーで決定
		if (inputMng.IsTrgDown(KEY_INPUT_RETURN))
		{
			// YESを選択している場合
			if (currentExitConfirm_ == ExitConfirmType::Yes)
			{
				// ゲーム終了を要求
				Application::GetInstance().RequestExit();
			}
			else
			{
				// NOなら終了確認を閉じる
				isExitConfirm_ = false;
			}
		}

		return;
	}

	// 上キー
	if (inputMng.IsTrgDown(KEY_INPUT_UP))
	{
		int menu = static_cast<int>(currentMenu_);

		// 一つ上のメニューへ移動
		menu--;

		// 一番上のメニューから更に上に行こうとした場合
		if (menu < 0)
		{
			// 一番下のメニューに移動
			menu = static_cast<int>(MenuType::Count) - 1;
		}

		currentMenu_ = static_cast<MenuType>(menu);
	}

	// 下キー
	if (inputMng.IsTrgDown(KEY_INPUT_DOWN))
	{
		int menu = static_cast<int>(currentMenu_);

		// 一つ下のメニューへ移動
		menu++;

		// 一番下のメニューから更に下に行こうとした場合
		if (menu >= static_cast<int>(MenuType::Count))
		{
			// 一番上に戻る
			menu = 0;
		}

		currentMenu_ = static_cast<MenuType>(menu);
	}

	// Enterキーで決定
	if (inputMng.IsTrgDown(KEY_INPUT_RETURN))
	{
		switch (currentMenu_)
		{
		case TitleScene::MenuType::GameStart:
			// 選曲画面へ
			scnMng_.ChangeScene(
				SceneManager::SCENE_ID::SONG_SELECT);
			break;
		case TitleScene::MenuType::Option:
			scnMng_.ChangeScene(SceneManager::SCENE_ID::SETTING);
			break;
		case TitleScene::MenuType::Exit:
			// 終了確認画面を表示する
			isExitConfirm_ = true;
			// 最初はNoを選択
			currentExitConfirm_ = ExitConfirmType::No;
			break;
		case TitleScene::MenuType::Count:
			break;
		default:
			break;
		}
	}
}

void TitleScene::Draw(void)
{
	// メニューで使用する色
	const unsigned int normalColor =
		GetColor(
			NORMAL_COLOR_R,
			NORMAL_COLOR_G,
			NORMAL_COLOR_B
		);

	const unsigned int selectedColor =
		GetColor(
			SELECTED_COLOR_R,
			SELECTED_COLOR_G,
			SELECTED_COLOR_B
		);

	const unsigned int panelColor =
		GetColor(
			PANEL_COLOR_R,
			PANEL_COLOR_G,
			PANEL_COLOR_B
		);

	const unsigned int selectBarColor =
		GetColor(
			SELECT_BAR_COLOR_R,
			SELECT_BAR_COLOR_G,
			SELECT_BAR_COLOR_B
		);

	// タイトル背景を描画
	if (titleBackgroundHandle_ != INVALID_GRAPH_HANDLE)
	{
		DrawExtendGraph(
			0,
			0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			titleBackgroundHandle_,
			true
		);
	}

	for (const auto& particle : titleParticles_)
	{
		particle.Draw();
	}

	// タイトルロゴを描画
	if (titleLogoHandle_ != INVALID_GRAPH_HANDLE)
	{
		// ロゴを画面中央に配置する
		const int logoPosX =
			(Application::SCREEN_SIZE_X - LOGO_WIDTH) / 2;

		DrawExtendGraph(
			logoPosX,
			LOGO_POS_Y,
			logoPosX + LOGO_WIDTH,
			LOGO_POS_Y + LOGO_HEIGHT,
			titleLogoHandle_,
			true
		);
	}

	// メニュー全体の半透明背景
	SetDrawBlendMode(
		DX_BLENDMODE_ALPHA,
		MENU_PANEL_ALPHA
	);

	DrawBox(
		MENU_PANEL_LEFT,
		MENU_PANEL_TOP,
		MENU_PANEL_RIGHT,
		MENU_PANEL_BOTTOM,
		panelColor,
		true
	);

	// 通常描画へ戻す
	SetDrawBlendMode(
		DX_BLENDMODE_NOBLEND,
		0
	);

	// 現在選択中のメニュー位置を計算
	const int selectedMenuY =
		MENU_START_Y +
		MENU_INTERVAL_Y *
		static_cast<int>(currentMenu_);

	// 選択中メニューの背景
	SetDrawBlendMode(
		DX_BLENDMODE_ALPHA,
		SELECT_BAR_ALPHA
	);

	DrawBox(
		SELECT_BAR_LEFT,
		selectedMenuY + SELECT_BAR_OFFSET_Y,
		SELECT_BAR_RIGHT,
		selectedMenuY + SELECT_BAR_OFFSET_Y + SELECT_BAR_HEIGHT,
		selectBarColor,
		true
	);

	// 通常描画へ戻す
	SetDrawBlendMode(
		DX_BLENDMODE_NOBLEND,
		0
	);

	// ゲームスタート
	DrawString(
		MENU_POS_X,
		MENU_START_Y,
		"ゲームスタート",
		currentMenu_ == MenuType::GameStart
		? selectedColor : normalColor
	);

	// 設定
	DrawString(
		MENU_POS_X,
		MENU_START_Y + MENU_INTERVAL_Y,
		"設定",
		currentMenu_ == MenuType::Option
		? selectedColor : normalColor
	);

	// 終了
	DrawString(
		MENU_POS_X,
		MENU_START_Y + MENU_INTERVAL_Y * 2,
		"終了",
		currentMenu_ == MenuType::Exit
		? selectedColor
		: normalColor
	);

	// カーソル
	const int cursorY =
		MENU_START_Y + MENU_INTERVAL_Y *
		static_cast<int>(currentMenu_);

	DrawString(
		CURSOR_POS_X,
		cursorY,
		">",
		selectedColor
	);

	// 終了確認画面
	if (isExitConfirm_)
	{
		const unsigned int windowColor = GetColor(30, 30, 30);
		const unsigned int normalColor = GetColor(255, 255, 255);
		const unsigned int selectedColor = GetColor(255, 255, 0);

		// 確認ウインドウ
		DrawBox(
			EXIT_WINDOW_LEFT,
			EXIT_WINDOW_TOP,
			EXIT_WINDOW_RIGHT,
			EXIT_WINDOW_BOTTOM,
			windowColor,
			true
		);

		// 確認メッセージ
		DrawString(
			EXIT_MESSAGE_POS_X,
			EXIT_MESSAGE_POS_Y,
			"ゲームを終了しますか？",
			normalColor
		);

		DrawString(
			EXIT_YES_POS_X,
			EXIT_YES_POS_Y,
			"はい",
			currentExitConfirm_ == ExitConfirmType::Yes
			? selectedColor
			: normalColor
		);

		DrawString(
			EXIT_NO_POS_X,
			EXIT_NO_POS_Y,
			"いいえ",
			currentExitConfirm_ == ExitConfirmType::No
			? selectedColor
			: normalColor
		);
	}
}

void TitleScene::Release(void)
{
	titleParticles_.clear();
}

void TitleScene::DrawShadowString(int x, int y, const char* text, unsigned int color) const
{
	// 文字の影
	const unsigned int shadowColor =
		GetColor(0, 0, 0);

	DrawString(
		x + TEXT_SHADOW_OFFSET_X,
		y + TEXT_SHADOW_OFFSET_Y,
		text,
		shadowColor
	);

	// 本来の文字
	DrawString(
		x,
		y,
		text,
		color
	);
}
