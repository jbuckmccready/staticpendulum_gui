TEMPLATE = lib
QT += core concurrent qml widgets
CONFIG += staticlib c++14 warn_on
QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS_RELEASE += -ffast-math -O3 -march=native
QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_LFLAGS_RELEASE += -static

# Qt deprecation warnings
DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    CoreEngine/cashkarp54.h \
    CoreEngine/pendulumsystem.h \
    CoreEngine/pendulummapintegrator.h \
    Models/pendulumsystemmodel.h \
    Models/integratormodel.h \
    Models/attractorlistmodel.h \
    Models/pendulummapmodel.h \
    QmlHelpers/systemintegrator.h \
    DataStorage/jsonreader.h \
    Models/modelsrepo.h

SOURCES += \
    CoreEngine/pendulumsystem.cpp \
    CoreEngine/pendulummapintegrator.cpp \
    Models/pendulumsystemmodel.cpp \
    Models/integratormodel.cpp \
    Models/attractorlistmodel.cpp \
    Models/pendulummapmodel.cpp \
    QmlHelpers/systemintegrator.cpp \
    DataStorage/jsonreader.cpp \
    Models/modelsrepo.cpp
