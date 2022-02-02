// SPDX-License-Identifier: LGPL-3.0-or-later
#include <cmath>
#include <thread>

#include <QElapsedTimer>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>

#include "Histogram.hxx"

//--- internal stuff ---

struct ARGBx2 {
    uint8_t b1, g1, r1, a1, b2, g2, r2, a2;
} __attribute__((packed));

//--- public constructors ---

Histogram::Histogram(const QImage &image, const bool &r, const bool &g, const bool &b,
                     const bool &a, const bool &dotted)
: _rstats{0}, _gstats{0}, _bstats{0}, _astats{0}, _image{},
  _histogram{Width, Height, QImage::Format_RGB32}, _max{0}, _factor{0}, _nsecs{0}, _chan_r{r},
  _chan_g{g}, _chan_b{b}, _chan_a{a}, _dotted{dotted}, _valid{false}
{
    create(image);
}

//--- public methods ---

bool Histogram::isValid() const noexcept
{
    return _valid;
}

Histogram::Result Histogram::get() const
{
    return {QPixmap::fromImage(_histogram), _valid};
}

qint64 Histogram::nsecs() const noexcept
{
    return _nsecs;
}

void Histogram::reset()
{
    std::fill(_rstats.begin(), _rstats.end(), 0);
    std::fill(_gstats.begin(), _gstats.end(), 0);
    std::fill(_bstats.begin(), _bstats.end(), 0);
    std::fill(_astats.begin(), _astats.end(), 0);

    _max = 0.0;
    _factor = 0.0;
    _valid = false;
}

bool Histogram::create(const QImage &image)
{
    reset();
    _image = image.convertToFormat(QImage::Format_ARGB32);
    _valid = calcStats();

    return redraw();
}

bool Histogram::redraw()
{
    _histogram.fill(Qt::white);
    _valid = _valid && calcMax() && drawScales() && drawScalesNumbers() && drawGraph();

    return _valid;
}

//--- protected methods ---

bool Histogram::calcStats()
{
    const GType size = _image.width() * _image.height() / 2;
    std::vector<std::thread> threads;
    QElapsedTimer timer;
    auto *ptr = reinterpret_cast<ARGBx2 *>(_image.bits());

    threads.reserve(4);

    timer.start();
    threads.emplace_back([this,ptr,size]{ std::for_each(ptr, ptr + size,
        [this](const ARGBx2 &c){ ++_rstats[c.r1]; ++_rstats[c.r2]; }); });
    threads.emplace_back([this,ptr,size]{ std::for_each(ptr, ptr + size,
        [this](const ARGBx2 &c){ ++_gstats[c.g1]; ++_gstats[c.g2]; }); });
    threads.emplace_back([this,ptr,size]{ std::for_each(ptr, ptr + size,
        [this](const ARGBx2 &c){ ++_bstats[c.b1]; ++_bstats[c.b2]; }); });
    threads.emplace_back([this,ptr,size]{ std::for_each(ptr, ptr + size,
        [this](const ARGBx2 &c){ ++_astats[c.a1]; ++_astats[c.a2]; }); });
    for (auto &thread : threads)
        thread.join();
    _nsecs = timer.nsecsElapsed();

    return true;
}

bool Histogram::calcMax()
{
    if (_chan_r || _chan_g || _chan_b || _chan_a)
    {
        _max = std::max({
            _chan_r ? *std::max_element(_rstats.begin(), _rstats.end()) : 0,
            _chan_g ? *std::max_element(_gstats.begin(), _gstats.end()) : 0,
            _chan_b ? *std::max_element(_bstats.begin(), _bstats.end()) : 0,
            _chan_a ? *std::max_element(_astats.begin(), _astats.end()) : 0
        });
        _factor = _max / GHeight;
        _factor = (_factor == 0.0) ? 1.0 : _factor;

        return true;
    }

    return false;
}

bool Histogram::drawScales()
{
    const auto CC = _chan_r + _chan_g + _chan_b + _chan_a;
    const auto GWidth = CSize * CC - 1;
    const auto GWidth25 = static_cast<GType>(GWidth * 0.25);
    const auto GWidth5 = static_cast<GType>(GWidth * 0.5);
    const auto GWidth75 = static_cast<GType>(GWidth * 0.75);
    QPainter p{&_histogram};

    p.setPen(Qt::black);

    // left scale arrow
    p.drawLine(GBorderX2 - 2, GBorder - 12, GBorderX2 - 2, Height - GBorder + 1);
    p.drawRect(GBorderX2 - 3, GBorder - 10, 2, 1);
    p.drawRect(GBorderX2 - 4, GBorder - 8, 4, 1);

    // left scales
    p.drawLine(GBorderX2 - 8, GTop, GBorderX2 - 2, GTop);
    p.drawLine(GBorderX2 - 5, GTop + GHeight25, GBorderX2 - 2, GTop + GHeight25);
    p.drawLine(GBorderX2 - 8, GTop + GHeight5, GBorderX2 - 2, GTop + GHeight5);
    p.drawLine(GBorderX2 - 5, GTop + GHeight75, GBorderX2 - 2, GTop + GHeight75);
    p.drawLine(GBorderX2 - 8, GTop + GHeight, GBorderX2 - 2, GTop + GHeight);

    // bottom scale arrow
    p.drawLine(GBorderX2 - 2, Height - GBorder + 1,
               GBorderX2 + CC * CSize + 12, Height - GBorder + 1);
    p.drawRect(GBorderX2 + CC * CSize + 9, Height - GBorder, 1, 2);
    p.drawRect(GBorderX2 + CC * CSize + 7, Height - GBorder - 1, 1, 4);

    // bottom scales
    p.drawLine(GBorderX2, GBorder + 2 + GHeight, GBorderX2, GBorder + 8 + GHeight);
    p.drawLine(GBorderX2 + GWidth25, GBorder + 2 + GHeight,
               GBorderX2 + GWidth25, GBorder + 5 + GHeight);
    p.drawLine(GBorderX2 + GWidth5, GBorder + 2 + GHeight,
               GBorderX2 + GWidth5, GBorder + 8 + GHeight);
    p.drawLine(GBorderX2 + GWidth75, GBorder + 2 + GHeight,
               GBorderX2 + GWidth75, GBorder + 5 + GHeight);
    p.drawLine(GBorderX2 + GWidth, GBorder + 2 + GHeight,
               GBorderX2 + GWidth, GBorder + 8 + GHeight);

    return true;
}

bool Histogram::drawScalesNumbers()
{
    const QFont font{"Monospace", 9};
    const QFontMetrics metrics{font};
    const QString max75 = QString::number(_max * 0.75, 'f', 0);
    const QString max5 = QString::number(_max * 0.5, 'f', 0);
    const QString max25 = QString::number(_max * 0.25, 'f', 0);
    const QString max = QString::number(_max, 'f', 0);
    const QString none = "0";
    const auto CC = _chan_r + _chan_g + _chan_b + _chan_a;
    const auto GWidth = CSize * CC - 1;
    const auto TRight = GBorderX2 - 10;
    const auto TTop = GBorder + GHeight + metrics.height() + 5;
    QPainter p{&_histogram};

    p.setFont(font);

    // left scale numbers
    p.drawText(TRight - metrics.horizontalAdvance(max), GBorder, max);
    p.drawText(TRight - metrics.horizontalAdvance(max75), GBorder + GHeight * 0.25, max75);
    p.drawText(TRight - metrics.horizontalAdvance(max5), GBorder + GHeight * 0.5, max5);
    p.drawText(TRight - metrics.horizontalAdvance(max25), GBorder + GHeight * 0.75, max25);
    p.drawText(TRight - metrics.horizontalAdvance(none), GBorder + GHeight, none);

    // bottom scale numbers
    p.drawText(GBorderX2, TTop, "0");
    p.drawText(GBorderX2 + static_cast<GType>(GWidth * 0.25), TTop,
               QString::number(std::floor(CMask * 0.25), 'f', 0));
    p.drawText(GBorderX2 + static_cast<GType>(GWidth * 0.5), TTop,
               QString::number(std::floor(CMask * 0.5), 'f', 0));
    p.drawText(GBorderX2 + static_cast<GType>(GWidth * 0.75), TTop,
               QString::number(std::floor(CMask * 0.75), 'f', 0));
    p.drawText(GBorderX2 + GWidth, TTop, QString::number(CMask));

    return true;
}

bool Histogram::drawGraph()
{
    const auto CC = _chan_r + _chan_g + _chan_b + _chan_a;
    QPainter p{&_histogram};

    if (_dotted)
    {
        for (GType i = 0; i < CMask; ++i)
        {
            const GType x = i * CC + GBorderX2;
            const GType x_offset = x + CC - 1;

            if (_chan_r)
            {
                p.setPen(Qt::red);
                p.drawLine(x, static_cast<GType>(GBottom - _rstats[i] / _factor),
                           x_offset, static_cast<GType>(GBottom - _rstats[i + 1] / _factor));
            }
            if (_chan_g)
            {
                p.setPen(Qt::green);
                p.drawLine(x + 1, static_cast<GType>(GBottom - _gstats[i] / _factor),
                           x_offset + 1, static_cast<GType>(GBottom - _gstats[i + 1] / _factor));
            }
            if (_chan_b)
            {
                p.setPen(Qt::blue);
                p.drawLine(x + 2, static_cast<GType>(GBottom - _bstats[i] / _factor),
                           x_offset + 2, static_cast<GType>(GBottom - _bstats[i + 1] / _factor));
            }
            if (_chan_a)
            {
                p.setPen(Qt::gray);
                p.drawLine(x + 3, static_cast<GType>(GBottom - _astats[i] / _factor),
                           x_offset + 3, static_cast<GType>(GBottom - _astats[i + 1] / _factor));
            }
        }
    }
    else
    {
        for (GType i = 0; i < CSize; ++i)
        {
            const GType x = i * CC + GBorderX2;

            if (_chan_r)
            {
                p.setPen(Qt::red);
                p.drawLine(x, static_cast<GType>(GBottom - _rstats[i] / _factor),
                           x, GBottom);
            }
            if (_chan_g)
            {
                p.setPen(Qt::green);
                p.drawLine(x + 1, static_cast<GType>(GBottom - _gstats[i] / _factor),
                           x + 1, GBottom);
            }
            if (_chan_b)
            {
                p.setPen(Qt::blue);
                p.drawLine(x + 2, static_cast<GType>(GBottom - _bstats[i] / _factor),
                           x + 2, GBottom);
            }
            if (_chan_a)
            {
                p.setPen(Qt::gray);
                p.drawLine(x + 3, static_cast<GType>(GBottom - _astats[i] / _factor),
                           x + 3, GBottom);
            }
        }
    }

    return true;
}