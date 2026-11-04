#include <cmath>
#include <fstream>
#include <sstream>
#include "ChartDataLoader.h"

bool ChartDataLoader::Load(
    const std::string& filePath, std::vector<ChartData>& chartDataList)
{
    // 前回の結果を残さない
    chartDataList.clear();

    // 指定されたCSVを開く
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    // 先頭の見出し行を飛ばす
    if (!std::getline(file, line))
    {
        return false;
    }

    // 全行が正常なら呼び出し元へ結果を渡す
    std::vector<ChartData> loadedChart;

    while (std::getline(file, line))
    {
        // 空白や改行は読み飛ばす
        if (line.find_first_not_of(" \t\r") == std::string::npos)
        {
            continue;
        }

        std::stringstream lineStream(line);
        std::string beatText;
        std::string laneText;

        // Beat列とLane列を取り出す
        if (!std::getline(lineStream, beatText, CSV_DELIMITER)
            || !std::getline(lineStream, laneText, CSV_DELIMITER))
        {
            return false;
        }

        ChartData chartData{};

        // 数値に変換する
        std::istringstream beatStream(beatText);
        std::istringstream laneStream(laneText);

        if (!(beatStream >> chartData.beat)
            || !(laneStream >> chartData.lane))
        {
            return false;
        }

        // 数値の後ろに空白以外の文字がないことを確認
        beatStream >> std::ws;
        laneStream >> std::ws;

        if (!beatStream.eof() || !laneStream.eof())
        {
            return false;
        }

        // 拍番号が有効な数値で開始拍以降か確認
        if (!std::isfinite(chartData.beat)
            || chartData.beat < FIRST_BEAT)
        {
            return false;
        }

        // 存在しないレーン番号を使っていないか確認
        if (chartData.lane < FIRST_LANE_INDEX
            || chartData.lane >= LANE_COUNT)
        {
            return false;
        }

        // 追加の列からノーツの種類を取得
		std::string typeText;
		std::getline(lineStream, typeText, CSV_DELIMITER);

        // 空白や改行を消す
        const auto firstCharacter =
			typeText.find_first_not_of(" \t\r");

		if (firstCharacter == std::string::npos)
		{
			typeText.clear();
		}
        else
        {
			const auto lastCharacter =
				typeText.find_last_not_of(" \t\r");

            typeText = typeText.substr(
                firstCharacter,
                lastCharacter - firstCharacter + 1
            );
        }

        // 最後の列から終了拍を取得
        std::string endBeatText;;
        std::getline(lineStream, endBeatText);

        if (typeText.empty() || typeText == "Tap")
        {
            // 種類が空欄の場合は通常ノーツにする
            chartData.type = NOTE_TYPE::NORMAL_NOTE;
			chartData.endBeat = chartData.beat;

            // 通常ノーツの終了拍は空欄にする
			if (endBeatText.find_first_not_of(" \t\r")
                != std::string::npos)
			{
				return false;
			}
        }
        else if (typeText == "Hold")
        {
            chartData.type = NOTE_TYPE::HOLD_NOTE;

			// ロングノーツには終了拍が必要
			std::istringstream endBeatStream(endBeatText);

			if (!(endBeatStream >> chartData.endBeat))
			{
				return false;
			}

            // 後ろの空白を読み飛ばす
            endBeatStream >> std::ws;

            // 終了拍は、開始拍より後の有効な数値
            if (!endBeatStream.eof()
                || !std::isfinite(chartData.endBeat)
                || chartData.endBeat <= chartData.beat)
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        loadedChart.push_back(chartData);
    }

    // ノーツがない譜面は失敗にする
    if (file.bad() || loadedChart.empty())
    {
        return false;
    }

    // 正常に読み込めた譜面を返す
    chartDataList.swap(loadedChart);
    return true;
}
