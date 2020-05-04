#-------------------------------------------------
#
# Project created by QtCreator 2016-08-12T18:14:23
#
#-------------------------------------------------

QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = rktoques
TEMPLATE = app

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT


SOURCES += main.cpp\
        mainwindow.cpp \
    procesodatos.cpp \
    datamanager.cpp \
    graficas.cpp \
    qcustomplot.cpp \
    utilidades.cpp \
    artistaseleccion.cpp \
    listaartistas.cpp \
    datos_tipos.cpp \
    listageneros.cpp \
    generoseleccion.cpp \
    espera.cpp

HEADERS  += mainwindow.h \
    procesodatos.h \
    definiciones.h \
    datamanager.h \
    graficas.h \
    qcustomplot.h \
    utilidades.h \
    artistaseleccion.h \
    listaartistas.h \
    datos_tipos.h \
    listageneros.h \
    generoseleccion.h \
    espera.h

FORMS    += mainwindow.ui \
    graficas.ui \
    artistaseleccion.ui \
    listaartistas.ui \
    listageneros.ui \
    generoseleccion.ui \
    espera.ui

RESOURCES +=


