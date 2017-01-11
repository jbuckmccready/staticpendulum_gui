QT       += testlib
QT       -= gui
TARGET = tst_intrinsics
CONFIG   += console c++14 warn_on
CONFIG   -= app_bundle
QMAKE_CXXFLAGS_RELEASE += -ffast-math -O3 -march=native
QMAKE_CXXFLAGS_RELEASE -= -O2

TEMPLATE = app

SOURCES += \
    tst_intrinsics.cpp
