// SPDX-License-Identifier: LGPL-3.0-or-later
#include <QRegularExpression>

#include "Image.hxx"

namespace Image
{
    Type filenameToType(const QString &str)
    {
        static QRegularExpression bmp_re{".*\\.([Bb][Mm][Pp])"};
        static QRegularExpression gif_re{".*\\.([Gg][Ii][Ff])"};
        static QRegularExpression jpeg_re{".*\\.([Jj][Pp][Ee]?[Gg])"};
        static QRegularExpression png_re{".*\\.([Pp][Nn][Gg])"};
        static QRegularExpression ppm_re{".*\\.([Pp][Pp][Mm])"};
        static QRegularExpression raw_re{".*\\.([Rr][Aa][Ww])"};
        static QRegularExpression xbm_re{".*\\.([Xx][Bb][Mm])"};
        static QRegularExpression xpm_re{".*\\.([Xx][Pp][Mm])"};

        if (QRegularExpressionMatch match{bmp_re.match(str)}; match.hasMatch())
            return Type::BMP;

        if (QRegularExpressionMatch match{gif_re.match(str)}; match.hasMatch())
            return Type::GIF;

        if (QRegularExpressionMatch match{jpeg_re.match(str)}; match.hasMatch())
            return Type::JPEG;

        if (QRegularExpressionMatch match{png_re.match(str)}; match.hasMatch())
            return Type::PNG;

        if (QRegularExpressionMatch match{ppm_re.match(str)}; match.hasMatch())
            return Type::PPM;

        if (QRegularExpressionMatch match{raw_re.match(str)}; match.hasMatch())
            return Type::RAW;

        if (QRegularExpressionMatch match{xbm_re.match(str)}; match.hasMatch())
            return Type::XBM;

        if (QRegularExpressionMatch match{xpm_re.match(str)}; match.hasMatch())
            return Type::XPM;

        return Type::Undefined;
    }
}