#pragma once
#include <string>
#include <vector>
#include "ChartData.h"

class ChartDataLoader
{
public:

	// 譜面のCSVを読み込む
	static bool Load(
		const std::string& filePath,
		std::vector<ChartData>& chartDataList
	);

private:

	// CSVの区切り文字
	static constexpr char CSV_DELIMITER = ',';

	// 譜面で使用できるレーン番号の範囲
	static constexpr int FIRST_LANE_INDEX = 0;
	static constexpr int LANE_COUNT = 4;

	// 譜面で指定できる最初の拍
	static constexpr double FIRST_BEAT = 0.0;
};

