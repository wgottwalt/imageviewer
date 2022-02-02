// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <QObject>
#include <QString>

namespace Support
{
    static const QString OrgName{"LinuxAddicted"};
    static const QString AppName{"ImageViewer"};
    static const QString VersionNumber{QString("0.1.0.0")};
    static const QString FullVersion{VersionNumber + "(" + __DATE__ + " " + __TIME__ + ")"};

    enum class Language {
        Dutch,
        English,
        French,
        Italian,
        German,
        Spanish
    };

    enum class Theme {
        Curve,
        Fusion,
        Mac,
        Windows
    };

    QString languageToString(const Language &lang) noexcept;
    Language stringToLanguage(const QString &str) noexcept;

    QString themeToString(const Theme &theme) noexcept;
    Theme stringToTheme(const QString &str) noexcept;
}

Q_DECLARE_METATYPE(Support::Language)
Q_DECLARE_METATYPE(Support::Theme)