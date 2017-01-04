TEMPLATE = lib
QT += core concurrent qml quick widgets quickcontrols2
CONFIG += staticlib c++14 warn_on
QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS_RELEASE += -ffast-math -O3
QMAKE_CXXFLAGS_RELEASE -= -O2

# Qt deprecation warnings
DEFINES += QT_DEPRECATED_WARNINGS;

# Import path added to help Qt Creator resolve QML modules
# actual import happens in the app main.cpp and imports the qml.qrc
QML_IMPORT_PATH += $$PWD/Qml

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

RESOURCES += qml.qrc
