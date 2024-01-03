TEMPLATE = app

CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

mingw:QMAKE_CXXFLAGS += -fopenmp -O3
mingw:QMAKE_LFLAGS +=  -fopenmp
mingw:LIBS += -fopenmp
mingw:message("g++")

msvc:QMAKE_CXXFLAGS+= -openmp /O2
msvc:QMAKE_LFLAGS += -openmp
msvc:LIBS += -openmp
msvc:message("msvc")

SOURCES += \
        main.cpp \
        params.cpp \
        solver.cpp \
        utils.cpp

INCLUDEPATH += \
    ../../dependency

HEADERS += \
    params.h \
    solver.h \
    utils.h

