# -------------------------------------------------
# Project created by QtCreator 2009-11-10T20:39:05
# -------------------------------------------------
QT += dbus
QT -= gui
LIBS += -lcwiid
TARGET = wiimotedev
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
UI_DIR = cache
MOC_DIR = cache
RCC_DIR = cache
OBJECTS_DIR = cache
SOURCES += src/main.cpp \
    src/wiimoteconnection.cpp \
    src/profilemanager.cpp \
    src/connectionmanager.cpp \
    src/uinput.cpp \
    src/datalogger.cpp
HEADERS += src/wiimoteconnection.h \
    src/profilemanager.h \
    src/connectionmanager.h \
    src/uinput.h \
    src/datalogger.h
