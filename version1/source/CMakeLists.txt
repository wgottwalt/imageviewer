# SPDX-License-Identifier: LGPL-3.0-or-later
# sources
SET                   (CXX_FILES Histogram.cxx Image.cxx Main.cxx MainWindow.cxx Support.cxx)
SET                   (HXX_FILES Histogram.hxx Image.hxx MainWindow.hxx Support.hxx)
SET                   (UI_FILES MainWindow.ui)
SET                   (RES_FILES Resources.qrc)
SET                   (TS_FILES i18n_Dutch.ts i18n_English.ts i18n_French.ts i18n_German.ts
                                i18n_Italian.ts i18n_Spanish.ts)

# wrapper and Qt automoc
QT6_ADD_TRANSLATION   (TS_SOURCES ${TS_FILES})
QT6_WRAP_CPP          (MOC_SOURCES ${HXX_FILES})
QT6_WRAP_UI           (UI_SOURCES ${UI_FILES})
QT6_ADD_RESOURCES     (RES_SOURCES ${RES_FILES})

# binaries
ADD_EXECUTABLE        (${CMAKE_PROJECT_NAME} ${CXX_FILES} ${MOC_SOURCES} ${UI_SOURCES}
                       ${RES_SOURCES} ${TS_SOURCES})
TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME} Qt6::Core Qt6::Widgets)
