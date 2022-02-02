// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <array>
#include <tuple>

#include <QImage>

class QGraphicsScene;

class Histogram {
public:
    //--- public types and constants ---
    using Result = std::tuple<QPixmap,bool>;
    using GType = int32_t;
    static constexpr GType Width = 1280;
    static constexpr GType Height = 720;
    static constexpr GType GBorder = 40; // graph border width
    static constexpr GType GBorderX2 = 2 * GBorder;
    static constexpr GType GHeight = Height - GBorderX2; // graph height
    static constexpr GType GHeight25 = GHeight * 0.25;
    static constexpr GType GHeight5 = GHeight * 0.5;
    static constexpr GType GHeight75 = GHeight * 0.75;
    static constexpr GType GTop = GBorder - 1;
    static constexpr GType GBottom = Height - GBorder - 1;
    static constexpr GType CSize = 256; // channel size
    static constexpr GType CMask = CSize - 1;

    //--- public constructors ---
    explicit Histogram(const QImage &image, const bool &r = true, const bool &g = true,
                       const bool &b = true, const bool &a = true, const bool &dotted = false);
    Histogram(const Histogram &rhs) = delete;
    Histogram(Histogram &&rhs) = delete;
    ~Histogram() = default;

    //--- public operators ---
    Histogram &operator=(const Histogram &rhs) = delete;
    Histogram &operator=(Histogram &&rhs) = delete;

    //--- public methods ---
    [[nodiscard]] bool isValid() const noexcept;
    [[nodiscard]] Result get() const;
    [[nodiscard]] qint64 nsecs() const noexcept;

    void reset();
    bool create(const QImage &image);
    bool redraw();

protected:
    //--- protected methods ---
    bool calcStats();
    bool calcMax();
    bool drawScales();
    bool drawScalesNumbers();
    bool drawGraph();

private:
    //--- private properties ---
    std::array<GType,CSize> _rstats;
    std::array<GType,CSize> _gstats;
    std::array<GType,CSize> _bstats;
    std::array<GType,CSize> _astats;
    QImage _image;
    QImage _histogram;
    double _max;
    double _factor;
    qint64 _nsecs;
    bool _chan_r;
    bool _chan_g;
    bool _chan_b;
    bool _chan_a;
    bool _dotted;
    bool _valid;
};