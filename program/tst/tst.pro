TEMPLATE = app

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

QT += testlib
QT -= gui

mingw:QMAKE_CXXFLAGS += -fopenmp -O3
mingw:QMAKE_LFLAGS +=  -fopenmp
mingw:LIBS += -fopenmp
mingw:message("g++")

msvc:QMAKE_CXXFLAGS+= -openmp /O2
msvc:QMAKE_LFLAGS += -openmp
msvc:LIBS += -openmp
msvc:message("msvc")

SOURCES +=  tst_common.cpp \
    ../cmd/params.cpp \
    ../cmd/utils.cpp

INCLUDEPATH += ../cmd/
INCLUDEPATH += \
    ../../dependency

HEADERS += \
    params.h \
    utils.h \
    tst_common.h
