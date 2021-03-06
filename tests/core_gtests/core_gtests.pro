include(../gtest_dependency.pri)
TEMPLATE = app
CONFIG += console c++14 warn_on
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt
QT += core
QMAKE_CXXFLAGS_RELEASE += -ffast-math -O3
QMAKE_CXXFLAGS_RELEASE -= -O2

HEADERS += \
    tst_cashkarp54.h

SOURCES += main.cpp \
    tst_cashkarp54.cpp

# Including core static library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../../src/core/ -lcore

INCLUDEPATH += $$PWD/../../src/core
DEPENDPATH += $$PWD/../../src/core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../src/core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../src/core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../src/core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../src/core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../src/core/libcore.a
