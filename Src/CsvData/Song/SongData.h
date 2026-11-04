#pragma once
#include<string>

struct SongData
{
	int id;					// 楽曲ID
	std::string title;		// 曲名
	int bpm;				// BPM
	std::string musicFile;	// 音源ファイル名
	std::string jacketFile;	// ジャケット画像ファイル名
	std::string chartFile;	// 譜面ファイルのパス
	double offsetSeconds = 0.0; // 曲の先頭に対する譜面の0拍目の時刻
};