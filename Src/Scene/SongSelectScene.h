#pragma once
#include <string>
#include <vector>
#include "../CsvData/Song/SongData.h"
#include "SceneBase.h"

class SongSelectScene : public SceneBase
{
public:

	SongSelectScene(void);				// コンストラクタ
	~SongSelectScene(void) override;	// デストラクタ

	void Init(void) override;			// 初期化
	void Update(void) override;			// 更新
	void Draw(void) override;			// 描画
	void Release(void) override;		// 解放

private:
	
	// タイトルへ戻るボタン
	static constexpr int TITLE_BUTTON_LEFT = 30;
	static constexpr int TITLE_BUTTON_TOP = 25;
	static constexpr int TITLE_BUTTON_RIGHT = 220;
	static constexpr int TITLE_BUTTON_BOTTOM = 75;

	// ジャケット仮表示位置
	static constexpr int JACKET_LEFT = 440;
	static constexpr int JACKET_TOP = 160;
	static constexpr int JACKET_RIGHT = 840;
	static constexpr int JACKET_BOTTOM = 560;

	// 現在選択している楽曲一覧
	int currentSongIndex_;

	// CSVから読み込んだ楽曲一覧
	std::vector<SongData> songs_;

	// 現在選択している楽曲を再生
	void PlaySelectedSong(void);

	// ボタンの上にマウスポインタがあるか
	bool IsMouseOnTitleButton(void) const;
};