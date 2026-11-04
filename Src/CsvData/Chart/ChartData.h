#pragma once

// 譜面に配置できるノーツの種類
enum class NOTE_TYPE
{
	NORMAL_NOTE, // 通常ノーツ
	HOLD_NOTE,   // ロングノーツ
};

struct ChartData
{
	// ノーツを押し始める拍番号
	double beat = 0.0;

	// レーン番号
	double endBeat = 0.0;

	// レーン番号
	int lane = 0;

	// ノーツの種類
	NOTE_TYPE type = NOTE_TYPE::NORMAL_NOTE;
};

