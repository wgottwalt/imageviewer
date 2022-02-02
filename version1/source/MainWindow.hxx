// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <QGraphicsScene>
#include <QImage>
#include <QMainWindow>
#include <QPalette>

#include "Support.hxx"

class QEvent;
class QSettings;
class QTranslator;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    //--- public constructors ---
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(const MainWindow &rhs) = delete;
    MainWindow(MainWindow &&rhs) = delete;
    ~MainWindow() override;

    //--- public operators ---
    MainWindow &operator=(const MainWindow &rhs) = delete;
    MainWindow &operator=(MainWindow &&rhs) = delete;

    //--- public methods ---

protected:
    //--- protected methods ---
    void changeEvent(QEvent *event) override;

    void setupLanguage();
    void setupTheme();
    void setupDarkmode();
    void setupDefaults();
    void setupActions();

    void setLanguage(const Support::Language &language = Support::Language::English);
    void setTheme(const Support::Theme &theme = Support::Theme::Fusion);
    void setDarkmode(const bool &on = true);

    void switchView();
    void updateView();
    void generateHistogram();

private:
    //--- private properties ---
    Ui::MainWindow *_ui;
    QTranslator *_translator;
    QSettings *_settings;
    QPalette _opalette;
    QPalette _dpalette;
    QGraphicsScene _scene;
    QGraphicsScene _histogram;
    QImage _image;
    Support::Language _language;
    Support::Theme _theme;
    bool _darkmode;
};