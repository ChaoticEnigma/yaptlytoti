## YAPTLYTOTI CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(YapTlyToti)

OPTION(YAP_CLIENT "Build YapTlyToti Client" ON)
OPTION(YAP_SERVER "Build YapTlyToti Server" OFF)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

ADD_SUBDIRECTORY(libchaos)
ADD_SUBDIRECTORY(voip)

IF(YAP_CLIENT)
    FIND_PACKAGE(Qt5Widgets 5.5 REQUIRED)
    FIND_PACKAGE(Qt5Multimedia 5.5 REQUIRED)
ENDIF()

### =================== SOURCES =================== ###
SET(Yap_SOURCES
    client/client.h

    client/main.cpp
    client/mainwindow.h
    client/mainwindow.cpp
    client/mainwindowslots.cpp

    client/audiosystem.h
    client/audiosystem.cpp
    client/inputreceiver.h
    client/inputreceiver.cpp

    client/settingsdialog.h
    client/settingsdialog.cpp
)

SET(YapD_SOURCES
    server/main.cpp

    server/server.h
    server/server.cpp
)

### =================== FORMS ===================== ###
SET(Yap_FORMS
    client/settingsdialog.ui
)

### =================== RESOURCES ================= ###
SET(Yap_RESOURCES
    client/mainwindow.qrc
)

### =================== FILES ===================== ###
SET(Yap_FILES
    LICENSE
    README.md
)

### =================== CONFIG ==================== ###

ADD_CUSTOM_TARGET(dummy SOURCES
    ${Yap_FILES}
)

INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    voip
    ${LibChaos_INCLUDE}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Multimedia_INCLUDE_DIRS}
)

IF(YAP_CLIENT)
    ADD_DEFINITIONS(${QT_DEFINITIONS})
    #SET(CMAKE_PREFIX_PATH "${QTDIR}; ${WINSDK_LIB}")
    GET_TARGET_PROPERTY(QtWidgets_LOCATION Qt5::Widgets LOCATION)
    GET_TARGET_PROPERTY(QtMultimedia_LOCATION Qt5::Multimedia LOCATION)
    MESSAGE(STATUS "Qt5Widgets: ${QtWidgets_LOCATION}")
    MESSAGE(STATUS "Qt5Multimedia: ${QtMultimedia_LOCATION}")

    SET(CLIENT_ALL)
ELSE()
    SET(CLIENT_ALL EXCLUDE_FROM_ALL)
ENDIF()

IF(YAP_SERVER)
    SET(SERVER_ALL)
ELSE()
    SET(SERVER_ALL EXCLUDE_FROM_ALL)
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibChaos_CXX_FLAGS}")
ADD_DEFINITIONS(-DBUILDING)

### =================== BUILD =================== ###

# Client
qt5_wrap_ui(Forms_SOURCES ${Yap_FORMS})
ADD_EXECUTABLE(yap ${CLIENT_ALL} ${Yap_SOURCES} ${Forms_SOURCES} ${Yap_RESOURCES})
TARGET_LINK_LIBRARIES(yap voip Qt5::Widgets Qt5::Multimedia)

# Server
ADD_EXECUTABLE(yapd ${SERVER_ALL} ${YapD_SOURCES})
TARGET_LINK_LIBRARIES(yapd voip)
