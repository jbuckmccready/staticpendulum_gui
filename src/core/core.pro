TEMPLATE = lib
include (../shared_config.pri)
QT += core concurrent
CONFIG += staticlib


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
