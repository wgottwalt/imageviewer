// SPDX-License-Identifier: LGPL-3.0-or-later
#include <QApplication>

#include "MainWindow.hxx"
#include "Support.hxx"

qint32 main(qint32 argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow main_win;

    QApplication::setApplicationName(Support::AppName);
    QApplication::setOrganizationName(Support::OrgName);
    QApplication::setApplicationVersion(Support::FullVersion);

    main_win.show();

    return QApplication::exec();
}
