## YAPTLYTOTI CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(yaptlytoti)

#ADD_SUBDIRECTORY(libchaos)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)

#FIND_PACKAGE(Qt5Core REQUIRED)
#FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Widgets 5.0 REQUIRED)

SET(yaptlytoti_HEADERS ### =================== HEADERS =================== ###
    main.h
    mainwindow.h
    mainworker.h
)
SET(yaptlytoti_SOURCES ### =================== SOURCES =================== ###
    main.cpp
    mainwindow.cpp
    mainwindowslots.cpp
    mainworker.cpp
)
SET(yaptlytoti_FORMS ### =================== FORMS =================== ###
    mainwindow.ui
)

### =================== CONFIG =================== ###

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
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

qt5_wrap_ui(Forms_SOURCES ${yaptlytoti_FORMS})

ADD_EXECUTABLE(yap ${yaptlytoti_HEADERS} ${yaptlytoti_SOURCES} ${Forms_SOURCES})
#TARGET_LINK_LIBRARIES(yap chaos)
TARGET_LINK_LIBRARIES(yap Qt5::Widgets)
