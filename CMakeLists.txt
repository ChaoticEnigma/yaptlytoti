## YAPTLYTOTI CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(YapTlyToti)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

FIND_PACKAGE(Qt5Widgets 5.5 REQUIRED)
FIND_PACKAGE(Qt5Network 5.5 REQUIRED)

### =================== SOURCES =================== ###
SET(Yap_SOURCES
    client/client.h
    client/mainwindow.h
    client/mainworker.h

    client/main.cpp
    client/mainwindow.cpp
    client/mainwindowslots.cpp
    client/mainworker.cpp
)

SET(YapD_SOURCES
    server/server.h

    server/main.cpp
    server/server.cpp
)

SET(VoIP_SOURCES
    voip/codec.h
    voip/codec.cpp
    voip/network.h
    voip/network.cpp
    voip/voip.h
    voip/voip.cpp
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
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/client
    ${CMAKE_BINARY_DIR}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
)

ADD_DEFINITIONS(${QT_DEFINITIONS})
#SET(CMAKE_PREFIX_PATH "${QTDIR}; ${WINSDK_LIB}")

GET_TARGET_PROPERTY(QtWidgets_LOCATION Qt5::Widgets LOCATION)
GET_TARGET_PROPERTY(QtNetwork_LOCATION Qt5::Network LOCATION)
MESSAGE(STATUS ${QtWidgets_LOCATION})
MESSAGE(STATUS ${QtNetwork_LOCATION})

ADD_DEFINITIONS(-DBUILDING)

### =================== BUILD =================== ###

# VoIP Library
ADD_LIBRARY(voip STATIC ${VoIP_SOURCES})
TARGET_LINK_LIBRARIES(voip Qt5::Network)

# Client
ADD_EXECUTABLE(yap ${Yap_SOURCES} ${Yap_RESOURCES})
TARGET_LINK_LIBRARIES(yap voip Qt5::Widgets)

# Server
ADD_EXECUTABLE(yapd ${YapD_SOURCES})
TARGET_LINK_LIBRARIES(yapd voip)


