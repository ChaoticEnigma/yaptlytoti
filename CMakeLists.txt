## YAPTLYTOTI CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(YapTlyToti)

#ADD_SUBDIRECTORY(libchaos)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

#FIND_PACKAGE(Qt5Core REQUIRED)
#FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Widgets 5.0 REQUIRED)

SET(Yap_HEADERS ### =================== HEADERS =================== ###
    client/main.h
    client/mainwindow.h
    client/mainworker.h
)
SET(Yap_SOURCES ### =================== SOURCES =================== ###
    client/main.cpp
    client/mainwindow.cpp
    #client/mainwindowslots.cpp
    client/mainworker.cpp
)
SET(Yap_RESOURCES ### =================== RESOURCES =================== ###
    client/mainwindow.qrc
)
SET(Yap_FORMS ### =================== FORMS =================== ###
    #client/mainwindow.ui
)
SET(Yap_FILES ### =================== FILES =================== ###
    LICENSE
    README.md
)

### =================== CONFIG =================== ###

ADD_CUSTOM_TARGET(dummy SOURCES
    ${Yap_FILES}
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/client
    ${CMAKE_BINARY_DIR}
    #${LibChaos_INCLUDE}
    ${Qt5Widgets_INCLUDE_DIRS}
)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibChaos_CXX_FLAGS}")

ADD_DEFINITIONS(${QT_DEFINITIONS})
SET(CMAKE_PREFIX_PATH "${QTDIR}; ${WINSDK_LIB}")

#GET_TARGET_PROPERTY(QtCore_LOCATION Qt5::Core LOCATION)
#GET_TARGET_PROPERTY(QtGui_LOCATION Qt5::Gui LOCATION)
GET_TARGET_PROPERTY(QtWidgets_LOCATION Qt5::Widgets LOCATION)
#MESSAGE(STATUS ${QtCore_LOCATION})
#MESSAGE(STATUS ${QtGui_LOCATION})
MESSAGE(STATUS ${QtWidgets_LOCATION})

### =================== BUILD =================== ###

ADD_DEFINITIONS(-DBUILDING)

#qt5_wrap_ui(Forms_SOURCES ${Yap_FORMS})

#ADD_EXECUTABLE(yap ${Yap_HEADERS} ${Yap_SOURCES} ${Forms_SOURCES})
ADD_EXECUTABLE(yap ${Yap_HEADERS} ${Yap_SOURCES} ${Yap_RESOURCES})
SET_PROPERTY(TARGET yap PROPERTY AUTORCC ON)
#TARGET_LINK_LIBRARIES(yap chaos)
TARGET_LINK_LIBRARIES(yap Qt5::Widgets)
