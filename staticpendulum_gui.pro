TEMPLATE = app
QT += core concurrent qml quick gui quickcontrols2
CONFIG += c++14 warn_on
QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS_RELEASE += -ffast-math -O3
QMAKE_CXXFLAGS_RELEASE -= -O2

HEADERS += \
    CoreEngine/cashkarp54.h \
    CoreEngine/pendulumsystem.h \
    CoreEngine/pendulummapintegrator.h \
    Models/pendulumsystemmodel.h \
    Models/integratormodel.h \
    Models/rootmodel.h \
    Models/attractorlistmodel.h \
    Models/pendulummapmodel.h \
    QmlHelpers/systemintegrator.h \

SOURCES += main.cpp \
    CoreEngine/pendulumsystem.cpp \
    CoreEngine/pendulummapintegrator.cpp \
    Models/pendulumsystemmodel.cpp \
    Models/integratormodel.cpp \
    Models/rootmodel.cpp \
    Models/attractorlistmodel.cpp \
    Models/pendulummapmodel.cpp \
    QmlHelpers/systemintegrator.cpp \

RESOURCES += qml.qrc
