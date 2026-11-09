#include <Windows.h>
#include <stdexcept>
#include <string>
#include "TimingSettings.h"

namespace
{
	// 設定ファイルの保存先と、INI内の項目名
	constexpr wchar_t APP_FOLDER[] = L"\\Rhythmia";
	constexpr wchar_t SETTINGS_FILE[] = L"\\Timing.ini";
	constexpr wchar_t TIMING_SECTION[] = L"Timing";
	constexpr wchar_t OFFSET_KEY[] = L"OffsetMs";

	// 補正値を文字列として読み込むバッファの長さ
	constexpr int OFFSET_TEXT_CAPACITY = 32;

	std::wstring GetSettingsPath(bool createFolder)
	{
		// Windowsユーザー専用の保存先を取得する
		const DWORD capacity =
			GetEnvironmentVariableW(L"LOCALAPPDATA", nullptr, 0);

		if (capacity == 0)
		{
			return {};
		}

		std::wstring folder(capacity, L'\0');
		const DWORD length = GetEnvironmentVariableW(
			L"LOCALAPPDATA", &folder[0], capacity
		);

		if (length == 0 || length >= capacity)
		{
			return {};
		}

		folder.resize(length);
		folder += APP_FOLDER;

		// 保存するときだけフォルダを作る、作成済みならそのまま使う
		if (createFolder && !CreateDirectoryW(folder.c_str(), nullptr)
			&& GetLastError() != ERROR_ALREADY_EXISTS)
		{
			return {};
		}

		return folder + SETTINGS_FILE;
	}
}

int TimingSettings::LoadMilliseconds()
{
	// 読み込む時だけは保存先フォルダを作らない
	const std::wstring path = GetSettingsPath(false);

	if (path.empty())
	{
		return DEFAULT_OFFSET_MS;
	}

	wchar_t value[OFFSET_TEXT_CAPACITY]{};

	GetPrivateProfileStringW(
		TIMING_SECTION,
		OFFSET_KEY,
		L"",
		value,
		OFFSET_TEXT_CAPACITY,
		path.c_str()
	);

	try
	{
		// 数字の後ろに余計な文字がある場合も初期値に戻す
		const std::wstring text = value;
		std::size_t parsedLength = 0;
		const int milliseconds = std::stoi(text, &parsedLength);

		if (parsedLength != text.size())
		{
			return DEFAULT_OFFSET_MS;
		}

		return ClampMilliseconds(milliseconds);
	}
	catch (const std::invalid_argument&)
	{
		return DEFAULT_OFFSET_MS;
	}
	catch (const std::out_of_range&)
	{
		return DEFAULT_OFFSET_MS;
	}
}

bool TimingSettings::SaveMilliseconds(int milliseconds)
{
	const std::wstring path = GetSettingsPath(true);

	if (path.empty())
	{
		return false;
	}

	// 許容範囲に収めてから書き込む
	const std::wstring value =
		std::to_wstring(ClampMilliseconds(milliseconds));

	return WritePrivateProfileStringW(
		TIMING_SECTION, OFFSET_KEY, value.c_str(), path.c_str()
	) != FALSE;
}

double TimingSettings::LoadSeconds()
{
	return LoadMilliseconds() / MILLISECONDS_PER_SECOND;
}

int TimingSettings::ClampMilliseconds(int milliseconds)
{
	if (milliseconds < -MAX_OFFSET_MS)
	{
		return -MAX_OFFSET_MS;
	}

	if (milliseconds > MAX_OFFSET_MS)
	{
		return MAX_OFFSET_MS;
	}

	return milliseconds;
}
