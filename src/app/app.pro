TEMPLATE = app
QT += core concurrent qml quick widgets quickcontrols2
CONFIG += c++14 warn_on
QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS_RELEASE += -ffast-math -O3
QMAKE_CXXFLAGS_RELEASE -= -O2
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = staticpendulum_gui

SOURCES += main.cpp

# Import path added to help Qt Creator resolve QML modules
# actual import happens in the app main.cpp and imports the qml.qrc
QML_IMPORT_PATH += $$PWD/Qml


# Including core static library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a

RESOURCES += qml.qrc
