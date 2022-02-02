// SPDX-License-Identifier: LGPL-3.0-or-later
#include <QApplication>
#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QImage>
#include <QSettings>
#include <QStyleFactory>
#include <QTranslator>

#include "MainWindow.hxx"
#include "ui_MainWindow.h"
#include "Histogram.hxx"
#include "Image.hxx"

//--- internal stuff ---

namespace I = Image;
namespace S = Support;

//--- public constructors ---

MainWindow::MainWindow(QWidget *parent)
: QMainWindow{parent}, _ui{new Ui::MainWindow}, _translator{new QTranslator{this}},
  _settings{new QSettings{S::OrgName, S::AppName, this}}, _opalette{QApplication::palette()},
  _dpalette{}, _scene{this}, _histogram{}, _image{}, _language{S::Language::English},
  _theme{S::Theme::Fusion}, _darkmode{false}
{
    _ui->setupUi(this);
    _ui->wid_graphics->setScene(&_scene);

    setupLanguage();
    setupTheme();
    setupDarkmode();
    setupDefaults();
    setupActions();

    if (_settings)
    {
        move(_settings->value("mainwindow_position", QPoint(40, 40)).toPoint());
        resize(_settings->value("mainwindow_size", QSize(800, 600)).toSize());
        _language = S::stringToLanguage(_settings->value("language", "English").toString());
        _theme = S::stringToTheme(_settings->value("theme", "Fusion").toString());
        _darkmode = _settings->value("darkmode", "false").toBool();
    }
    else
    {
        move(40, 40);
        resize(800, 600);
    }
    setLanguage(_language);
    setTheme(_theme);
    setDarkmode(_darkmode);
}

MainWindow::~MainWindow()
{
    if (_settings)
    {
        _settings->setValue("mainwindow_position", pos());
        _settings->setValue("mainwindow_size", size());
        _settings->setValue("language", S::languageToString(_language));
        _settings->setValue("theme", S::themeToString(_theme));
        _settings->setValue("darkmode", _darkmode);
    }

    delete _settings;
    delete _translator;
    delete _ui;
}

//--- protected methods ---

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        _ui->retranslateUi(this);

    QMainWindow::changeEvent(event);
}

void MainWindow::setupLanguage()
{
    connect(_ui->me_language_english, &QAction::triggered, [this]{ setLanguage(); });
    connect(_ui->me_language_dutch, &QAction::triggered,
            [this]{ setLanguage(S::Language::Dutch); });
    connect(_ui->me_language_french, &QAction::triggered,
            [this]{ setLanguage(S::Language::French); });
    connect(_ui->me_language_german, &QAction::triggered,
            [this]{ setLanguage(S::Language::German); });
    connect(_ui->me_language_italian, &QAction::triggered,
            [this]{ setLanguage(S::Language::Italian); });
    connect(_ui->me_language_spanish, &QAction::triggered,
            [this]{ setLanguage(S::Language::Spanish); });
}

void MainWindow::setupTheme()
{
    _ui->me_theme_fusion->setVisible(false);
    _ui->me_theme_mac->setVisible(false);
    _ui->me_theme_curve->setVisible(false);
    _ui->me_theme_windows->setVisible(false);

    for (const auto &stylename : QStyleFactory::keys())
    {
        if (stylename == "Fusion")
        {
            _ui->me_theme_fusion->setVisible(true);
            connect(_ui->me_theme_fusion, &QAction::triggered, [this]{ setTheme(); });
            continue;
        }

        if (stylename == "MacOS")
        {
            _ui->me_theme_mac->setVisible(true);
            connect(_ui->me_theme_mac, &QAction::triggered,
                    [this]{ setTheme(S::Theme::Mac); });
            continue;
        }

        if (stylename == "QtCurve")
        {
            _ui->me_theme_curve->setVisible(true);
            connect(_ui->me_theme_curve, &QAction::triggered,
                    [this]{ setTheme(S::Theme::Curve); });
            continue;
        }

        if (stylename == "Windows")
        {
            _ui->me_theme_windows->setVisible(true);
            connect(_ui->me_theme_windows, &QAction::triggered,
                    [this]{ setTheme(S::Theme::Windows); });
        }
    }
}

void MainWindow::setupDarkmode()
{
    _dpalette.setColor(QPalette::Window, QColor(53, 53, 53));
    _dpalette.setColor(QPalette::WindowText, Qt::white);
    _dpalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    _dpalette.setColor(QPalette::Base, QColor(42, 42, 42));
    _dpalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    _dpalette.setColor(QPalette::ToolTipBase, Qt::white);
    _dpalette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
    _dpalette.setColor(QPalette::Text, Qt::white);
    _dpalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    _dpalette.setColor(QPalette::Dark, QColor(30, 30, 30));
    _dpalette.setColor(QPalette::Shadow, QColor(0, 0, 0));
    _dpalette.setColor(QPalette::Button, QColor(76, 76, 76));
    _dpalette.setColor(QPalette::ButtonText, Qt::white);
    _dpalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    _dpalette.setColor(QPalette::BrightText, Qt::red);
    _dpalette.setColor(QPalette::Link, QColor(42, 130, 218));
    _dpalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    _dpalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    _dpalette.setColor(QPalette::HighlightedText, Qt::white);
    _dpalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));

    connect(_ui->me_theme_darkmode, &QAction::toggled,
            [this](const bool state){ setDarkmode(state); });
}

void MainWindow::setupDefaults()
{
    _ui->box_factor->setDecimals(2);
    _ui->box_factor->setValue(1.0);
    _ui->box_factor->setRange(0.10, 10.0);
    _ui->box_factor->setSingleStep(0.01);
}

void MainWindow::setupActions()
{
    // menus
    connect(_ui->me_app_open, &QAction::triggered,
        [this]
        {
            if (auto filename = QFileDialog::getOpenFileName(this, tr("I18N_IMAGEOPEN"), "./",
                tr("I18N_IMAGEOPEN_FILTER")); !filename.isEmpty())
            {
                if (auto type = I::filenameToType(filename); (type != I::Type::RAW) &&
                    (type != I::Type::Undefined))
                {
                    _image.load(filename);
                    switchView();
                }
            }
        }
    );
    connect(_ui->me_app_quit, &QAction::triggered, this, &MainWindow::close);
    connect(_ui->me_about_qt, &QAction::triggered, QApplication::instance(), &QApplication::aboutQt);

    // buttons
    connect(_ui->btn_histogram, &QToolButton::clicked, [this]{ switchView(); });
    connect(_ui->btn_r, &QToolButton::clicked, [this]{ switchView(); });
    connect(_ui->btn_g, &QToolButton::clicked, [this]{ switchView(); });
    connect(_ui->btn_b, &QToolButton::clicked, [this]{ switchView(); });
    connect(_ui->btn_a, &QToolButton::clicked, [this]{ switchView(); });
    connect(_ui->btn_dot, &QToolButton::clicked, [this]{ switchView(); });
    connect(_ui->box_factor, &QDoubleSpinBox::valueChanged, this, &MainWindow::updateView);
}

void MainWindow::setLanguage(const S::Language &language)
{
    auto *translator = new(std::nothrow) QTranslator(this);
    bool success;

    if (!translator)
        return;

    switch (_language)
    {
        case S::Language::Dutch:
            success = translator->load(":/Language/Dutch");
            break;

        case S::Language::French:
            success = translator->load(":/Language/French");
            break;

        case S::Language::German:
            success = translator->load(":/Language/German");
            break;

        case S::Language::Italian:
            success = translator->load(":/Language/Italian");
            break;

        case S::Language::Spanish:
            success = translator->load(":/Language/Spanish");
            break;

        case S::Language::English:
        default:
            success = translator->load(":/Language/English");
    }

    if (success && !translator->isEmpty())
    {
        _language = language;
        _ui->me_language_dutch->setEnabled(true);
        _ui->me_language_english->setEnabled(true);
        _ui->me_language_french->setEnabled(true);
        _ui->me_language_german->setEnabled(true);
        _ui->me_language_italian->setEnabled(true);
        _ui->me_language_spanish->setEnabled(true);

        switch (language)
        {
            case S::Language::Dutch:
                _ui->me_language_dutch->setEnabled(false);
                break;

            case S::Language::French:
                _ui->me_language_french->setEnabled(false);
                break;

            case S::Language::German:
                _ui->me_language_german->setEnabled(false);
                break;

            case S::Language::Italian:
                _ui->me_language_italian->setEnabled(false);
                break;

            case S::Language::Spanish:
                _ui->me_language_spanish->setEnabled(false);
                break;

            case S::Language::English:
            default:
                _ui->me_language_english->setEnabled(false);
        }

        QApplication::removeTranslator(_translator);
        delete _translator;
        _translator = translator;
        QApplication::installTranslator(_translator);
    }
    else
        delete translator;
}

void MainWindow::setTheme(const S::Theme &theme)
{
    _ui->me_theme_curve->setEnabled(true);
    _ui->me_theme_fusion->setEnabled(true);
    _ui->me_theme_mac->setEnabled(true);
    _ui->me_theme_windows->setEnabled(true);

    switch (theme)
    {
        // XXX: the QtCurve Qt5 plugin may be broken, it does not handle
        //      QPalette::ColorRole properly
        case S::Theme::Curve:
#if 0
            QApplication::setStyle(QStyleFactory::create("QtCurve"));
            _ui->me_theme_qtcurve->setEnabled(false);
#endif
            break;

        case S::Theme::Mac:
            QApplication::setStyle(QStyleFactory::create("MacOS"));
            _ui->me_theme_mac->setEnabled(false);
            break;

        case S::Theme::Windows:
            QApplication::setStyle(QStyleFactory::create("Windows"));
            _ui->me_theme_windows->setEnabled(false);
            break;

        case S::Theme::Fusion:
        default:
            QApplication::setStyle(QStyleFactory::create("Fusion"));
            _ui->me_theme_fusion->setEnabled(false);
    }
}

void MainWindow::setDarkmode(const bool &on)
{
    _darkmode = on;
    QApplication::setPalette(on ? _dpalette : _opalette);
    _ui->me_theme_darkmode->setChecked(on);
}

void MainWindow::switchView()
{
    _scene.clear();

    if (_ui->btn_histogram->isChecked())
    {
        generateHistogram();
        _ui->wid_graphics->setScene(&_histogram);
    }
    else
    {
        _scene.setSceneRect(0, 0, _image.width(), _image.height());
        _scene.addPixmap(QPixmap::fromImage(_image));
        _ui->wid_graphics->setScene(&_scene);
    }

    _ui->lab_ores_val->setText(QString::number(_image.width()) + "x" +
                               QString::number(_image.height()));

    updateView();
}

void MainWindow::updateView()
{
    const auto scale = _ui->btn_histogram->isChecked() ? 1.0 : _ui->box_factor->value();

    _ui->wid_graphics->resetTransform();
    _ui->wid_graphics->scale(scale, scale);

    _ui->lab_nres_val->setText(QString::number(_image.width() * scale, 'f', 0) + "x" +
                               QString::number(_image.height() * scale, 'f', 0));
}

void MainWindow::generateHistogram()
{
    Histogram h{_image, _ui->btn_r->isChecked(), _ui->btn_g->isChecked(),
                _ui->btn_b->isChecked(), _ui->btn_a->isChecked(), _ui->btn_dot->isChecked()};
    auto [pixmap, valid] = h.get();

    if (valid)
    {
        _histogram.clear();
        _histogram.addPixmap(pixmap);
        _ui->wid_statusbar->showMessage(QString::number(h.nsecs()));
    }
}