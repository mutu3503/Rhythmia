#pragma once
#include <vector>
#include "../Effect/TitleParticle.h"
#include "SceneBase.h"

class TitleScene : public SceneBase
{
public:

	TitleScene(void);			// コンストラクタ
	~TitleScene(void) override;	// デストラクタ

	void Init(void) override;	// 初期化
	void Update(void) override;	// 更新
	void Draw(void) override;	// 描画
	void Release(void) override;// 解放

private:

	enum class MenuType
	{
		GameStart,
		Option,
		Exit,
		Count,
	};

	enum class ExitConfirmType
	{
		Yes,	// 終了する
		No,		// 終了しない
		Count
	};

	// 終了確認で現在選択している項目
	ExitConfirmType currentExitConfirm_;

	// 現在選択しているメニュー
	MenuType currentMenu_;

	// タイトル画面のパーティクル
	std::vector<TitleParticle> titleParticles_;

	// 画像読み込み失敗時のハンドル
	static constexpr int INVALID_GRAPH_HANDLE = -1;

	// タイトルロゴのサイズ・位置
	static constexpr int LOGO_WIDTH = 600;
	static constexpr int LOGO_HEIGHT = 340;
	static constexpr int LOGO_POS_Y = 10;

	// メニュー表示
	static constexpr int MENU_POS_X = 560;
	static constexpr int MENU_START_Y = 390;
	static constexpr int MENU_INTERVAL_Y = 50;
	static constexpr int CURSOR_POS_X = MENU_POS_X - 40;

	// メニュー文字より少し広めに取る
	static constexpr int MENU_PANEL_LEFT = MENU_POS_X - 70;
	static constexpr int MENU_PANEL_TOP = MENU_START_Y - 30;
	static constexpr int MENU_PANEL_RIGHT = MENU_POS_X + 250;

	// 「終了」まで含めた高さ
	static constexpr int MENU_PANEL_BOTTOM =
		MENU_START_Y +
		MENU_INTERVAL_Y * 2 + 50;

	// 半透明度
	static constexpr int MENU_PANEL_ALPHA = 150;

	// 選択中メニューの背景
	static constexpr int SELECT_BAR_LEFT = MENU_PANEL_LEFT + 10;
	static constexpr int SELECT_BAR_RIGHT = MENU_PANEL_RIGHT - 10;

	static constexpr int SELECT_BAR_HEIGHT = 35;
	static constexpr int SELECT_BAR_OFFSET_Y = -5;

	static constexpr int SELECT_BAR_ALPHA = 90;

	// 文字の影
	static constexpr int TEXT_SHADOW_OFFSET_X = 2;
	static constexpr int TEXT_SHADOW_OFFSET_Y = 2;

	// 色
	static constexpr int COLOR_MAX = 255;

	static constexpr int NORMAL_COLOR_R = 255;
	static constexpr int NORMAL_COLOR_G = 255;
	static constexpr int NORMAL_COLOR_B = 255;

	static constexpr int SELECTED_COLOR_R = 255;
	static constexpr int SELECTED_COLOR_G = 230;
	static constexpr int SELECTED_COLOR_B = 80;

	static constexpr int PANEL_COLOR_R = 5;
	static constexpr int PANEL_COLOR_G = 10;
	static constexpr int PANEL_COLOR_B = 30;

	static constexpr int SELECT_BAR_COLOR_R = 120;
	static constexpr int SELECT_BAR_COLOR_G = 160;
	static constexpr int SELECT_BAR_COLOR_B = 255;

	// 終了確認ウィンドウ
	static constexpr int EXIT_WINDOW_LEFT = 390;
	static constexpr int EXIT_WINDOW_TOP = 250;
	static constexpr int EXIT_WINDOW_RIGHT = 890;
	static constexpr int EXIT_WINDOW_BOTTOM = 470;

	// 終了確認メッセージ
	static constexpr int EXIT_MESSAGE_POS_X = 510;
	static constexpr int EXIT_MESSAGE_POS_Y = 300;

	// YES
	static constexpr int EXIT_YES_POS_X = 500;
	static constexpr int EXIT_YES_POS_Y = 380;

	// NO
	static constexpr int EXIT_NO_POS_X = 700;
	static constexpr int EXIT_NO_POS_Y = 380;

	// タイトル画面に表示するパーティクル数
	static constexpr int TITLE_PARTICLE_COUNT = 40;

	// タイトル背景画像
	int titleBackgroundHandle_;

	// タイトルロゴ画像
	int titleLogoHandle_;

	// 終了確認画面を表示しているか
	bool isExitConfirm_;

	// 影付き文字を描画する
	void DrawShadowString(
		int x,
		int y,
		const char* text,
		unsigned int color
	) const;
};

