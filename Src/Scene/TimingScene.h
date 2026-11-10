#pragma once
#include "SceneBase.h"
#include "../Manager/TimingSettings.h"

class TimingScene : public SceneBase
{
public:

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// 矢印を1回押したときの変更幅
	static constexpr int OFFSET_STEP_MS = 10;

	// 編集中の補正値はエンターキーを押すまでファイルには保存しない
	int offsetMs_ = TimingSettings::DEFAULT_OFFSET_MS;

	// 保存の結果を表示
	const char* statusText_ = "";
};

