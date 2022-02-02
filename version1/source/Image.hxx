// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <QObject>
#include <QString>

namespace Image
{
    enum class Type : quint8 {
        Undefined,
        BMP,
        GIF,
        JPEG,
        PNG,
        PPM,
        RAW,
        XBM,
        XPM
    };

    Type filenameToType(const QString &str);
}

Q_DECLARE_METATYPE(Image::Type)