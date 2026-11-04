#pragma once
#include <string>
#include <vector>
#include "SongData.h"

class SongDataLoader
{
public:

	// CSVファイルから楽曲データを読み込む
	static std::vector<SongData> Load(
		const std::string& filePath
	);
};