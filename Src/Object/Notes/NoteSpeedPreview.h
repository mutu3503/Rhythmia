#pragma once
#include <chrono>

class NoteSpeedPreview
{
public:

	// 設定画面を開いた状態からプレビューを始める
	void Init(int noteImageHandle);

	// 編集中の速度でプレビューを描画
	void Draw(double pixelsPerSecond) const;

private:

	static constexpr int INVALID_IMAGE_HANDLE = -1;
	int noteImageHandle_ = INVALID_IMAGE_HANDLE;

	// 経過時間でノーツを動かす
	std::chrono::steady_clock::time_point startedAt_{};
};

