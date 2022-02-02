// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Support.hxx"

namespace Support
{
    QString languageToString(const Language &lang) noexcept
    {
        switch (lang)
        {
            case Language::Dutch:
                return "Dutch";

            case Language::French:
                return "French";

            case Language::Italian:
                return "Italian";

            case Language::German:
                return "German";

            case Language::Spanish:
                return "Spanish";

            case Language::English:
            default:
                return "English";
        }
    }

    Language stringToLanguage(const QString &str) noexcept
    {
        if (str == "Dutch")
            return Language::Dutch;

        if (str == "French")
            return Language::French;

        if (str == "Italian")
            return Language::Italian;

        if (str == "German")
            return Language::German;

        if (str == "Spanish")
            return Language::Spanish;

        return Language::English;
    }

    QString themeToString(const Theme &theme) noexcept
    {
        switch (theme)
        {
            case Theme::Curve:
                return "Curve";

            case Theme::Windows:
                return "Windows";

            case Theme::Fusion:
            default:
                return "Fusion";
        }
    }

    Theme stringToTheme(const QString &str) noexcept
    {
        if (str == "Curve")
            return Theme::Curve;

        if (str == "Windows")
            return Theme::Windows;

        return Theme::Fusion;
    }
}