#pragma once
#include "SceneBase.h"

class SettingScene : public SceneBase
{
public:

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// メニュー項目の並び順
	enum class MenuItem
	{
		Timing,
		NoteSpeed,
		Back,
		Count
	};

	// 編集中の速度
	int noteSpeedPercent_ = 0;
	const char* speedStatusText_ = "";

	// 画面を開いたときはタイミング調整を選択
	MenuItem selectedItem_ = MenuItem::Timing;
};

