## YAPTLYTOTI CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(LibVOIP)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)

#ADD_SUBDIRECTORY(libchaos)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Opus REQUIRED)
FIND_PACKAGE(Qt5Network 5.5 REQUIRED)

### =================== SOURCES =================== ###

SET(VoIP_SOURCES
    codec.h
    codec.cpp
    network.h
    network.cpp
    voip.h
    voip.cpp
    voipmessage.h
    voipmessage.cpp
)

### =================== CONFIG ==================== ###

INCLUDE_DIRECTORIES(
    ${LibChaos_INCLUDE}
    ${Qt5Network_INCLUDE_DIRS}
    ${OPUS_INCLUDE_DIRS}
)

MESSAGE(STATUS "libopus: ${OPUS_INCLUDE_DIRS} ${OPUS_LIBRARIES}")

ADD_DEFINITIONS(${QT_DEFINITIONS})
#SET(CMAKE_PREFIX_PATH "${QTDIR}; ${WINSDK_LIB}")
GET_TARGET_PROPERTY(QtNetwork_LOCATION Qt5::Network LOCATION)
MESSAGE(STATUS "Qt5Network: ${Qt5Network_INCLUDE_DIRS} ${QtNetwork_LOCATION}")

#MESSAGE(STATUS ${LibChaos_CXX_FLAGS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibChaos_CXX_FLAGS}")
ADD_DEFINITIONS(-DBUILDING)

### =================== BUILD =================== ###

# VoIP Library
ADD_LIBRARY(voip STATIC ${VoIP_SOURCES})
TARGET_LINK_LIBRARIES(voip chaos Qt5::Network ${OPUS_LIBRARIES})
