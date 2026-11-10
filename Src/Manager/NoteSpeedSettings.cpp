#include "PlayerSettingsFile.h"
#include "NoteSpeedSettings.h"

namespace
{
    // タイミング補正と同じファイルの、別の項目に保存
    constexpr wchar_t SECTION[] = L"Notes";
    constexpr wchar_t SPEED_KEY[] = L"SpeedPercent";
}

int NoteSpeedSettings::ClampPercent(int percent)
{
    if (percent < MIN_PERCENT)
    {
        return MIN_PERCENT;
    }

    if (percent > MAX_PERCENT)
    {
        return MAX_PERCENT;
    }

    return percent;
}

int NoteSpeedSettings::LoadPercent(void)
{
    const auto path = PlayerSettingsFile::GetPath(false);

    if (path.empty())
    {
        return DEFAULT_PERCENT;
    }

    // 未設定の値は初期速度に戻す
    const auto percent = GetPrivateProfileIntW(
        SECTION, SPEED_KEY, DEFAULT_PERCENT, path.c_str()
    );

    if (percent < MIN_PERCENT || percent > MAX_PERCENT)
    {
        return DEFAULT_PERCENT;
    }

    return static_cast<int>(percent);
}

bool NoteSpeedSettings::SavePercent(int percent)
{
    const auto path = PlayerSettingsFile::GetPath(true);

    if (path.empty())
    {
        return false;
    }

    const auto value = std::to_wstring(ClampPercent(percent));

    // 速度の項目だけを置き換える
    return WritePrivateProfileStringW(SECTION, SPEED_KEY,
        value.c_str(),
        path.c_str()
    )!= FALSE;
}

double NoteSpeedSettings::LoadPixelsPerSeconds(void)
{
    return BASE_PIXELS_PER_SECOND * LoadPercent() / DEFAULT_PERCENT;
}
