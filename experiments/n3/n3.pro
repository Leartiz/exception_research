TEMPLATE = app

CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fopenmp -O3
QMAKE_LFLAGS +=  -fopenmp
LIBS += -fopenmp

SOURCES += \
        main.cpp
