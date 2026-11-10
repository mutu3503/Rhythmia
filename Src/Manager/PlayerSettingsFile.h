#pragma once
#include <Windows.h>
#include <string>

namespace PlayerSettingsFile
{
	// 既存のタイミング設定を引き継ぐため、保存先の名前は変更しない
	inline std::wstring GetPath(bool createFolder)
	{
		constexpr wchar_t APP_FOLDER[] = L"\\Rhythmia";
		constexpr wchar_t SETTINGS_FILE[] = L"\\Timing.ini";

		// ローカル設定フォルダを取得する
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

		// 保存するときだけ、必要に応じてフォルダを作る
		if (createFolder && !CreateDirectoryW(folder.c_str(), nullptr)
			&& GetLastError() != ERROR_ALREADY_EXISTS)
		{
			return {};
		}

		return folder + SETTINGS_FILE;
	}
}
