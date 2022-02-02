# SPDX-License-Identifier: LGPL-3.0-or-later
CMAKE_MINIMUM_REQUIRED  (VERSION 3.0)
PROJECT                 (ImageViewer LANGUAGES CXX)

# packages
FIND_PACKAGE            (Qt6 COMPONENTS Core Widgets LinguistTools REQUIRED)

# includes and output paths
SET                     (CMAKE_INCLUDE_CURRENT_DIR on)
INCLUDE_DIRECTORIES     (${PROJECT_SOURCE_DIR}/source)
SET                     (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
SET                     (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/plugin)
SET                     (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# compiler settings
IF                      (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    MESSAGE             ("Building for Linux")
    SET                 (CMAKE_CXX_FLAGS "-std=c++20 -pthread -flto -fPIC -W -Wall -Wextra -Os -s")
    SET                 (CMAKE_C_FLAGS "-std=c17 -pthread -flto -fPIC -W -Wall -Wextra -Os -s")
ELSE                    ()
    MESSAGE             (FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
ENDIF                   ()

# sources
ADD_SUBDIRECTORY        (source)
