#include <fstream>
#include <sstream>
#include <cmath>
#include "SongDataLoader.h"

std::vector<SongData> SongDataLoader::Load(const std::string& filePath)
{
	constexpr char CSV_DELIMITER = ',';

	// 読み込んだ楽曲データを保存する
	std::vector<SongData> songDataList;

	// CSVファイルを開く
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		return songDataList;
	}

	std::string line;

	// 1行目は飛ばす
	std::getline(file, line);

	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::stringstream lineStream(line);
		std::string value;

		SongData songData{};

		// ID
		std::getline(lineStream, value, CSV_DELIMITER);

		// 空欄じゃなかったら数値に変換
		if (!value.empty())
		{
			songData.id = std::stoi(value);
		}

		// Title
		std::getline(lineStream, songData.title, CSV_DELIMITER);
		// Bpm
		std::getline(lineStream, value, CSV_DELIMITER);

		if (!value.empty())
		{
			songData.bpm = std::stoi(value);
		}

		// MusicFile
		std::getline(lineStream, songData.musicFile, CSV_DELIMITER);
		// JacketFile
		std::getline(lineStream, songData.jacketFile, CSV_DELIMITER);

		// 譜面ファイルのパスを取得
		std::getline(lineStream,
			songData.chartFile,
			CSV_DELIMITER
		);

		// 最後の列として譜面の調整値
		std::string offsetText;
		std::getline(lineStream, offsetText);

		// 調整値が空欄だったら初期値を使う
		if (offsetText.find_first_not_of(" \t\r") != std::string::npos)
		{
			std::istringstream offsetStream(offsetText);

			if (!(offsetStream >> songData.offsetSeconds))
			{
				continue;
			}

			// 数値の後ろに空白以外がないか確認
			offsetStream >> std::ws;

			if (!offsetStream.eof() ||
				!std::isfinite(songData.offsetSeconds))
			{
				continue;
			}
		}

		songDataList.push_back(songData);
	}

	// CSVを閉じる
	file.close();

	return songDataList;
}
